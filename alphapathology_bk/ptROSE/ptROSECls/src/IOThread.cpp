#include "IOThread.h"

namespace ALPHA
{
	namespace ROSE 
	{
		std::mutex IOThread::m_mutex;
		std::list<patch> IOThread::m_patchList;
	}
}

cv::Mat sobel(const cv::Mat& img)
{
	cv::Mat img_gray;
	cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
	cv::Mat gradX, gradY, out;
	Sobel(img_gray, gradX, CV_16S, 1, 0, 3);
	convertScaleAbs(gradX, gradX);
	Sobel(img_gray, gradY, CV_16S, 0, 1, 3);
	convertScaleAbs(gradY, gradY);
	addWeighted(gradX, 0.5, gradY, 0.5, 0, out);
	threshold(out, out, 0, 255, cv::THRESH_OTSU);
	return std::move(out);
}

float calVar(const cv::Mat& img)
{
	cv::Mat img_gray;
	cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
	cv::Mat img_flat = img_gray.reshape(0, 1);
	float sum = 0;
	float mean = 0;
	int len = img_flat.size().width;
	for (int i = 0; i < len; i++){
		sum += img_flat.at<uchar>(0, i);
	}
	mean = sum / len;

	sum = 0;
	for (int i = 0; i < len; i++)
		sum += pow(img_flat.at<uchar>(0, i) - mean, 2);
	return sum / len;
}

int countContuor(const cv::Mat& img)
{
	int count = 0;
	for (int i = 0; i < img.rows; i++)
	{
		const uchar* inPtr = img.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++)
		{
			if (inPtr[j] == 255)
				count++;
		}
	}
	return count;
}

float detectBlur(const cv::Mat& img)
{
	cv::Mat srcBlurImg;
	GaussianBlur(img, srcBlurImg, cv::Size(15, 15), 0, 0);

	cv::Mat sobelImg = sobel(img);
	cv::Mat sobelBlurImg = sobel(srcBlurImg);

	cv::Mat sobelImgOpen;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	morphologyEx(sobelImg, sobelImgOpen, cv::MORPH_OPEN, element);

	int contour = countContuor(sobelImgOpen);
	int contourBlur = countContuor(sobelBlurImg);
	float changeRate = (contour - contourBlur) * 1.0f / contour;
	return abs(changeRate);
}

cv::Mat ALPHA::ROSE::IOThread::readPatch(int x_start, int y_start, int width, int height, int level)
{
	unsigned char* dest = new unsigned char[width * height * 4];

	openslide_read_region(m_slide, (uint32_t*)dest, x_start*pow(2, level), y_start * pow(2, level), level, width, height);
	cv::Mat patch_img(height, width, CV_8UC3, cv::Scalar(0));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// BGRA
			unsigned char* pixel = dest + (i * width + j) * 4;
			patch_img.at<cv::Vec3b>(i, j)[0] = *pixel;
			patch_img.at<cv::Vec3b>(i, j)[1] = *(pixel + 1);
			patch_img.at<cv::Vec3b>(i, j)[2] = *(pixel + 2);
		}
	}
	delete[] dest;
	return std::move(patch_img);
}

void ALPHA::ROSE::IOThread::start()
{
	std::thread* t = new std::thread(std::bind(&IOThread::run, this));
	m_run_thread = t;
}

void ALPHA::ROSE::IOThread::wait()
{
	m_run_thread->join();
}

ALPHA::ROSE::IOThread::IOThread(IFPipelinePathologyIn* input, int start_row, int start_col, int cut_width, int cut_height)
{
	m_svs_path = input->m_svs_path;
	m_slide_name = input->m_slide_name;
	m_start_row = start_row;
	m_start_col = start_col;
	m_cut_width = cut_width;
	m_cut_height = cut_height;
	m_slide = openslide_open(m_svs_path.c_str());
};

ALPHA::ROSE::IOThread::~IOThread()
{
	openslide_close(m_slide);
	if (m_run_thread != nullptr)
		delete m_run_thread;
}

void  ALPHA::ROSE::IOThread::run()
{
	int64_t slide_width = 0;
	int64_t slide_height = 0;
	int level = 3;
	openslide_get_level_dimensions(m_slide, level, &slide_width, &slide_height);
	LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Slide size: " << slide_width << "X" << slide_height);
	int width = slide_width;
	int height = slide_height;
	add_size(width,height);
	int width_num = slide_width / m_cut_width;
	int height_num = slide_height / m_cut_height;
	bool allIOComplete = false;
	LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Patch num: " << width_num << "X" << height_num);
	int count = 0;
	double time;
	for (int h_idx = m_start_row; h_idx < height_num; h_idx += 2)
	{
		for (int w_idx = m_start_col; w_idx < width_num; w_idx +=2)
		{
			count++;
			int w_start = w_idx * m_cut_width;
			int h_start = h_idx * m_cut_height;
			clock_t start, end;
			start = clock();
			cv::Mat img = readPatch(w_start, h_start, m_cut_width, m_cut_height, level);
			end = clock();
			double endtime = (double)(end - start) / CLOCKS_PER_SEC;
			time += endtime;
			if (img.empty())
				m_num_broken++;
			//else if (detectBlur(img) < 1)
			//	m_num_blur++;
			//else if (calVar(img) < 60)
			//	m_num_blank++;
			else
			{
				add_patch(patch{ { w_start * pow(2, level) , h_start * pow(2, level)}, img });
				m_num_images ++;
			}
		}
	}

	LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Patch time: " << time << "Patch number: " << count);
}

void  ALPHA::ROSE::IOThread::add_patch(patch&& p)
{
	std::lock_guard<std::mutex> g(m_mutex);
	m_patchList.emplace_back(p);
}
void  ALPHA::ROSE::IOThread::add_size(int &w,int & h)
{
	width = w;
	height = h;
}

ALPHA::ROSE::patch ALPHA::ROSE::IOThread::get_patch()
{
	std::lock_guard<std::mutex> g(m_mutex);
	ALPHA::ROSE::patch p = m_patchList.front();
	m_patchList.pop_front();
	return std::move(p);
}

int ALPHA::ROSE::IOThread::get_patchlist_size()
{
	return m_patchList.size();
}