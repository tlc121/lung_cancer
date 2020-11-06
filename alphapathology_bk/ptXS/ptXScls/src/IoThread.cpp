#include "IOThread.h"

cv::Mat readPatch(openslide_t* input, int x_start, int y_start, int width, int height, int level)
{
	unsigned char* dest = new unsigned char[width * height * 4];

	openslide_read_region(input, (uint32_t*)dest, x_start, y_start, level, width, height);
	cv::Mat patch_img(height, width, CV_8UC3, cv::Scalar(0));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			unsigned char* pixel = dest + (i * width + j) * 4;
			patch_img.at<cv::Vec3b>(i, j)[0] = *pixel;
			patch_img.at<cv::Vec3b>(i, j)[1] = *(pixel + 1);
			patch_img.at<cv::Vec3b>(i, j)[2] = *(pixel + 2);
		}
	}
	delete[] dest;
	return std::move(patch_img);
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
	for (int i = 0; i < len; i++) {
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

void ALPHA::XS::IOThread::startThread()
{
	std::thread* t = new std::thread(std::bind(&IOThread::run, this));
	m_run_thread = t;
}

void  ALPHA::XS::IOThread::run()
{

	int64_t w_slide = 0;
	int64_t h_slide = 0;
	openslide_get_level_dimensions(m_slide, 0, &w_slide, &h_slide);
	LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Slide size: " << w_slide << "X" << h_slide);

	int width_num = w_slide / m_cutWidth;
	int height_num = h_slide / m_cutHeight;

	

	for (int h_idx = 0; h_idx < height_num; h_idx++)
	{
		for (int w_idx = 0; w_idx < width_num; w_idx++)
		{
			int w_start = w_idx * m_cutWidth;
			int h_start = h_idx * m_cutHeight;
			cv::Mat img = readPatch(m_slide, w_start, h_start, m_cutWidth, m_cutHeight, 0);

			if (img.empty()) {
				m_num_broken++;
				continue;
			}
			/*if (calVar(img) < 60) {
				m_num_blank++;
				
			}*/
			m_num_images++;
	
			/*if (detectBlur(img) < 1) {
				m_num_blur++;
			}*/

			add_patch(patch{ { w_start , h_start }, img });
		}
	}
	//m_ratio_blur = m_num_blur * 1.0 / (width_num * height_num);
	m_flag = true; // cut svs finish
}

void  ALPHA::XS::IOThread::add_patch(patch&& p)
{
	std::lock_guard<std::mutex> g(m_mutex);
	m_patchList.emplace_back(p);
}

ALPHA::XS::patch ALPHA::XS::IOThread::get_patch()
{
	std::lock_guard<std::mutex> g(m_mutex);
	ALPHA::XS::patch p = m_patchList.front();
	m_patchList.pop_front();
	return std::move(p);
}

int ALPHA::XS::IOThread::get_patchlist_size()
{
	return m_patchList.size();
}