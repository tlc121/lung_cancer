#include "Pathoutils.h"
#include "CommonUtils.h"
#include "itkBinaryImageToShapeLabelMapFilter.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stack>
#include <iostream>
//#include <memory>
//#include <fstream>  
//#include <sstream>  
//#include <errno.h>
//#include <algorithm>
//#include <limits>
//#include <iomanip>
//#include <math.h>

#ifdef WIN32  
//#include <windows.h>  
//#include <direct.h>  
#else  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <dirent.h>  
#endif 

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h> 
#include <log4cplus/layout.h> 
#include <log4cplus/loggingmacros.h> 
#include <log4cplus/helpers/stringhelper.h> 

#include "libconfig.h++"


namespace ALPHA {
	namespace COMM
	{

		Pathoutils::Pathoutils(const std::string& imgpath) {
			img_path = imgpath;

			}

		Pathoutils::~Pathoutils() {}

		std::vector<unsigned int> Pathoutils::get_bboxes() {
			return bboxes;
		}

		std::vector<std::pair<unsigned int, unsigned int>> Pathoutils::get_points() {
			return points;
		}



		
		//return code 0 means ok
		//return code 1 means image could not be opened
		//return code 2 means cell area could not be detected
		int Pathoutils::getArea() {
			cv::Mat img;
			img = cv::imread(img_path, 2);

			/*cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
			cv::imshow("Display", img);
			cv::waitKey(0);*/

			//make sure img could be opend
			if (img.empty())
			{
				LOG4CPLUS_WARN(COMM::MyLogger::getInstance()->m_rootLog, "Could not open image: " << img_path);
				LOG4CPLUS_WARN(COMM::MyLogger::getInstance()->m_rootLog, "Is it broken? ");
				return 1;
			}

			
			int height = img.size().height;
			int width = img.size().width;

			int cut_width = (0.75 - 0.41) * width;
			int row_cut = 0.41 * width;
			cv::Mat roi_area = img(cv::Rect2i(row_cut, 0, cut_width, height));
			

			//denoise
			cv::Mat blur_img; 
			cv::GaussianBlur(roi_area, blur_img, cv::Size(13, 13), 0);

		

			//sharpen
			cv::Mat sharpen_img;
			cv::Mat sharpen_kernel = (cv::Mat_<float>(3, 3) <<
				0, -2, 0,
				-2, 9, -2,
				0, -2, 0);
			cv::filter2D(blur_img, sharpen_img, -1, sharpen_kernel);


			//canny filters
			cv::Mat canny_edge;
			int low_threshold = 0;
			int max_threshold = 26;
			cv::Canny(sharpen_img, canny_edge, low_threshold, max_threshold);


			//hough circle
			std::vector<cv::Vec3f> circles;
			int min_radius = 0.034 * width;
			int max_radius = 0.138 * width;
			cv::HoughCircles(canny_edge, circles, cv::HOUGH_GRADIENT, 1, 50, 100, 80, min_radius, max_radius);


			//discard some circles which are not in the right position
			std::vector<cv::Vec3f> circle_set;

			//circle position info
			int radius_thre = 0.075 * width;
			int col_upper = 0.8 * cut_width;
			int row_upper = 0.721 * height;
			int col_lower = 0.6 * cut_width;
			int row_lower = 0.412 * height;

			for (auto circle : circles) {
				int col_pos = circle[0];
				int row_pos = circle[1];
				int radius = circle[2];
				//std::cout << radius << std::endl;
				if ((radius >= radius_thre) && (row_pos > row_lower && row_pos < row_upper) && (col_pos > col_lower && col_pos < col_upper)) {
					circle_set.push_back(circle);
				}
			}

			//if no circle detected means target is not clear
			if (circle_set.size() == 0) {
				isClear = true;
				return 2;
			}
			
			cv::Vec3f result_cir = circle_set[0];
			if (circle_set.size() > 1) {
				int max_radius = 0;
				for (auto circle : circle_set) {
					int radius_temp = circle[2];
					if (radius_temp > max_radius) {
						result_cir = circle;
						max_radius = radius_temp;
					}
				}
			}
		

			//4 positions of bboxings
			unsigned int minx, miny, maxx, maxy;
			unsigned int center_y = result_cir[1];
			unsigned int center_x = result_cir[0] + row_cut;
			unsigned int result_radius = result_cir[2];

			minx = center_x - result_radius - 0.015 * width;
			miny = center_y - result_radius - 0.015 * width;
			maxx = center_x + result_radius + 0.015 * width;
			maxy = center_y + result_radius + 0.015 * width;

			bboxes.push_back(minx);
			bboxes.push_back(miny);
			bboxes.push_back(maxx);
			bboxes.push_back(maxy);

			//3 point 
			std::pair<unsigned int, unsigned int> point1;
			std::pair<unsigned int, unsigned int> point2;
			std::pair<unsigned int, unsigned int> point3;

			point1 = std::make_pair(minx+0.33*2*result_radius, miny+0.33*2*result_radius);
			point2 = std::make_pair(minx + 0.66 * 2 * result_radius, miny + 0.33 * 2 * result_radius);
			point3 = std::make_pair(minx + 0.5 * 2 * result_radius, miny + 0.66 * 2 * result_radius);

			points.push_back(point1);
			points.push_back(point2);
			points.push_back(point3);

			return 0;
		}



		
	}
}



