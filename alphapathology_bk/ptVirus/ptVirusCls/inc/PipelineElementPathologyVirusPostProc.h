#pragma once
#include "opencv2/opencv.hpp"

namespace ALPHA
{
	namespace VIRUS
	{
		class Post_processing
		{
		public:
			Post_processing() {};
			~Post_processing() {};

			void bgr2hsv_pixel(float R, float G, float B, float& H, float& S, float& V);
			//void bgr2hsv(float R, float G, float B, float& H, float& S, float& V);
			cv::Mat bgr2hsv_image(const cv::Mat bgr_img);
			cv::Mat rm_bg(const cv::Mat bgr_img);
			cv::Mat channel_one2three(const cv::Mat one_c);
			std::string get_color(const cv::Mat img);
			bool get_sample_quality(const cv::Mat in_img, float ratio_threshold = 0.1);
			cv::Mat get_channel(const cv::Mat in_img, const std::string color = "green");
			float get_cell_ratio(const cv::Mat red_c);

		private:
			std::map<std::string, std::vector<int>> color_list = {
				{ "white"  ,{ 0, 0, 221, 180, 30, 255 } },
				{ "red"    ,{ 156, 43, 46, 180, 255, 255 } },
				{ "red2"   ,{ 0, 43, 46, 10, 255, 255 } },
				{ "orange" ,{ 11, 43, 46, 25, 255, 255 } },
				{ "yellow" ,{ 26, 43, 46, 32, 255, 255 } },
				{ "green"  ,{ 33, 43, 46, 77, 255, 255 } },
				{ "cyan"   ,{ 78, 43, 46, 99, 255, 255 } },
				{ "blue"   ,{ 100, 43, 46, 124, 255, 255 } },
				{ "purple" ,{ 125, 43, 46, 155, 255, 255 } }
			};
		};

	}
}