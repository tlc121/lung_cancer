#include "IoThread.h"


namespace ALPHA
{
	namespace TCT {

		std::mutex IoThread::m_mutex;
		std::list<patch> IoThread::m_patchList;
		bool IoThread::pause_flag = false;

		IoThread::IoThread(openslide_t* slide, int cut_width, int cut_height, int start_pos_h, int start_pos_w) {
			this->slide = slide;
			this->cut_width = cut_width;
			this->cut_height = cut_height;
			this->m_start_pos_h = start_pos_h;
			this->m_start_pos_w = start_pos_w;
			int64_t w_slide = 0;
			int64_t h_slide = 0;
			openslide_get_level_dimensions(slide, 0, &w_slide, &h_slide);
			m_width_num = (w_slide) / cut_width;
			m_height_num = (h_slide) / cut_height;
		}

		void IoThread::startThread()
		{
			std::thread* t = new std::thread(std::bind(&IoThread::run, this));
			m_run_thread = t;
		}


		float IoThread::cal_var(std::vector<int> im_flat)
		{
			float sum = 0;
			float mean = 0;
			int len = im_flat.size();
			for (int i = 0; i < len; i++)
				sum += im_flat[i];
			mean = sum / len;

			sum = 0;
			for (int i = 0; i < len; i++)
				sum += pow(im_flat[i] - mean, 2);
			return sum / len;
		}


		void IoThread::run()
		{
			for (int h_idx = m_start_pos_h; h_idx < m_height_num; h_idx+=2) {
				for (int w_idx = m_start_pos_w; w_idx < m_width_num; w_idx+=2)
				{

					if (get_patch_count() > 500)
					{
						::Sleep(3000);
					}

			

					cv::Mat img;
					int h_start = h_idx * cut_height;
					int w_start = w_idx * cut_width;

					/*if (w_start == 20640 && h_start == 66880) {
						std::cout << "xxxx" << std::endl;
					}*/
					img = this->read_patch(slide, w_start, h_start, cut_width, cut_height, 0);
					cv::Mat img_qc_GRAY;
					cv::cvtColor(img, img_qc_GRAY, cv::COLOR_BGR2GRAY);
					cv::Mat img_flat = img_qc_GRAY.reshape(0, 1);
					if (cal_var(img_flat) < 60) {
						continue;
					}
					patch temp;
					temp.array = img;
					temp.coord = std::pair<int, int>(h_start, w_start);
					this->add_patch(temp);
				}
			}
			stop_flag = true;
		}

		cv::Mat IoThread::read_patch(openslide_t* input, int x_start, int y_start, int width, int height, int level) {

			unsigned char* dest = new unsigned char[width * height * 4];

			openslide_read_region(input, (uint32_t*)dest, x_start, y_start, level, width, height);
			cv::Mat patch_img(height, width, CV_8UC3, cv::Scalar(0));

			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					unsigned char* pixel_m = dest + (i * width * 4) + (j * 4);
					patch_img.at<cv::Vec3b>(i, j)[0] = *(pixel_m + 0);
					patch_img.at<cv::Vec3b>(i, j)[1] = *(pixel_m + 1);
					patch_img.at<cv::Vec3b>(i, j)[2] = *(pixel_m + 2);
				}
			}

			delete dest;
			return patch_img;
		}


		void IoThread::pause() {
			m_mutex.lock();
			pause_flag = true;
			m_mutex.unlock();
		}

		void IoThread::start() {
			m_mutex.lock();
			pause_flag = false;
			m_mutex.unlock();
		}


		void IoThread::add_patch(patch img)
		{
			m_mutex.lock();
			(m_patchList).push_back(img);
			m_mutex.unlock();
		}

		patch IoThread::get_patch() {
			m_mutex.lock();
			patch ret = (m_patchList).front();
			(m_patchList).pop_front();
			m_mutex.unlock();
			return ret;
		}
	}
}