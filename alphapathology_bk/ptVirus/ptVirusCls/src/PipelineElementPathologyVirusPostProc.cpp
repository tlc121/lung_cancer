#include <algorithm>
#include <opencv2/opencv.hpp>
#include "PipelineElementPathologyVirusPostProc.h"
//#include "CommonUtils.h"

namespace ALPHA {
	namespace VIRUS {

	void Post_processing::bgr2hsv_pixel(float R, float G, float B, float & H, float & S, float & V)
	{
		//if (R == NULL || G == NULL || B == NULL)
		//	LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "internel check");

		float min, max, delta, tmp;
		float R_norm = R / 255.0;
		float G_norm = G / 255.0;
		float B_norm = B / 255.0;
		tmp = std::min(R_norm, G_norm); 
		min = std::min(tmp, B_norm);
		tmp = std::max(R_norm, G_norm);
		max = std::max(tmp, B_norm);

		delta = max - min;

		if (max == min)
			H = 0;
		else if (max == R_norm)
			H = int((G_norm - B_norm) / (delta) * 60 + 360) % 360;
		else if (max == G_norm)
			H = int(60 * ((B_norm - R_norm) / delta) + 120) % 360;
		else if (max == B_norm)
			H = int(60 * ((R_norm - G_norm) / delta) + 240) % 360;
		//else
		//	LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "internel check delta value: "<<delta);


		if (max == 0)
			S = 0;
		else
			S = delta / max;

		V = max;
		H = int(H / 360 * 179);
		S = int(S * 255);
		V = int(V * 255);
	}

	cv::Mat Post_processing::bgr2hsv_image(const cv::Mat bgr_img)
	{
		cv::Mat hsv_img = cv::Mat(bgr_img.rows, bgr_img.cols, CV_8UC3, cv::Scalar(255));
		for (int i = 1; i < bgr_img.rows; i++)
		{
			float cPointR, cPointG, cPointB, cPointH, cPointS, cPointV;
			
			for (int j = 1; j < bgr_img.cols; j++)
			{
				
				cPointB = bgr_img.at<cv::Vec3b>(i, j)[0];
				cPointG = bgr_img.at<cv::Vec3b>(i, j)[1];
				cPointR = bgr_img.at<cv::Vec3b>(i, j)[2];
				bgr2hsv_pixel(cPointR, cPointG, cPointB, cPointH, cPointS, cPointV);
				hsv_img.at<cv::Vec3b>(i, j)[0] = cPointH;
				hsv_img.at<cv::Vec3b>(i, j)[1] = cPointS;
				hsv_img.at<cv::Vec3b>(i, j)[2] = cPointV;
			}
		}
		return hsv_img;
	}

	cv::Mat Post_processing::rm_bg(const cv::Mat bgr_img)
	{
		cv::Mat tmp = cv::Mat(bgr_img.rows, bgr_img.cols, CV_8UC1, cv::Scalar(255));
		cv::Mat out = cv::Mat(bgr_img.rows, bgr_img.cols, CV_8UC3, cv::Scalar(255));
		cv::Mat green_c = get_channel(bgr_img, "green");
		//LOG4CPLUS_DEBUG(COMM::MyLogger::getInstance()->m_rootLog, "get channel done");
		float otsu_threshold = cv::threshold(green_c, tmp, 0, 255, cv::THRESH_OTSU) * 1.3;
		//LOG4CPLUS_DEBUG(COMM::MyLogger::getInstance()->m_rootLog, "get otsu_thre: "<< otsu_threshold);
		float safe_threshold = 60;
		cv::threshold(green_c, tmp, std::max(otsu_threshold, safe_threshold), 255, cv::THRESH_BINARY);
		cv::bitwise_and(bgr_img, channel_one2three(tmp), out);
		//LOG4CPLUS_DEBUG(COMM::MyLogger::getInstance()->m_rootLog, "remove background done");
		return out;
	}

	cv::Mat Post_processing::channel_one2three(const cv::Mat one_c)
	{
		cv::Mat three_c = cv::Mat::zeros(one_c.rows, one_c.cols, CV_8UC3);
		std::vector<cv::Mat> channels;
		for (int i = 0; i<3; i++)
		{
			channels.push_back(one_c);
		}
		cv::merge(channels, three_c);
		return three_c;
	}

	std::string Post_processing::get_color(const cv::Mat img)
	{
		//cv::Mat hsv_img = cv::Mat(img.rows, img.cols, CV_8UC3, cv::Scalar(255));
        //cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV,0); // not worked here!!!
		cv::Mat hsv_img = bgr2hsv_image(img);
		//LOG4CPLUS_DEBUG(COMM::MyLogger::getInstance()->m_rootLog, "to hsv done");
		int maxsum = 0;
		std::string color = "black";

		for (auto item = color_list.begin(); item != color_list.end(); item ++)
		{
			auto k = item->first;
			auto v = item->second;
			cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
			std::vector<std::vector<cv::Point> > contours;
			std::vector<cv::Vec4i> hierarchy;
			int sum = 0;
			//LOG4CPLUS_DEBUG(COMM::MyLogger::getInstance()->m_rootLog, "find contours");
			cv::inRange(hsv_img, cv::Scalar(v.at(0), v.at(1), v.at(2)), cv::Scalar(v.at(3), v.at(4), v.at(5)), mask);
			cv::findContours(mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
			//LOG4CPLUS_DEBUG(COMM::MyLogger::getInstance()->m_rootLog, "calc conours areas");
			//if (contours.empty())
			//	continue;

			for (int i = 0; i < contours.size(); i++)
			{
				//if (contours.at(i).empty())
				//	continue;
				sum += cv::contourArea(contours.at(i));
			}
			if (sum > maxsum)
			{
				maxsum = sum;
				color = k;
			}
		}
		return color;
	}

	cv::Mat Post_processing::get_channel(const cv::Mat bgr_img, const std::string color)
	{
		cv::Mat channel = cv::Mat(bgr_img.rows, bgr_img.cols, CV_8UC1, cv::Scalar(255));
		std::vector<cv::Mat> bgr_channels;
		cv::split(bgr_img, bgr_channels);

		int clr;
		if (color == "blue")
			clr = 0;
		else if (color == "green")
			clr = 1;
		else if (color == "red")
			clr = 2;
		else
			throw std::invalid_argument("Image Channel error");

		return bgr_channels[clr];
	}

	float Post_processing::get_cell_ratio(const cv::Mat red_c)
	{
		cv::Mat tmp = cv::Mat(red_c.rows, red_c.cols, CV_8UC1, cv::Scalar(255));
		// use otsu to compute a threshold
		float otsu_threshold = cv::threshold(red_c, tmp, 0, 255, cv::THRESH_OTSU);
		float safe_threshold = 15;
		// binary thresholding
		cv::threshold(red_c, tmp, cv::max(otsu_threshold, safe_threshold), 255, cv::THRESH_BINARY);
		double cell_ratio = cv::sum(tmp)[0] / (255 * tmp.cols * tmp.rows);
		return cell_ratio;
	}

	bool Post_processing::get_sample_quality(const cv::Mat in_img, float ratio_threshold)
	{
		cv::Mat red_c = get_channel(in_img, "red");
		float cell_ratio = get_cell_ratio(red_c);
		if (cell_ratio > ratio_threshold)
			return true;
		else
			return false;
	}

	//// closing
	//Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	//morphologyEx(tmp, tmp, MORPH_CLOSE, element);
	//imwrite("E:\\test2.bmp", tmp);
	}
}