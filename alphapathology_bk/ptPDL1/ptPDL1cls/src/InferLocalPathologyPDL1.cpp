//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : PDL1
//  @ File Name : InferLocalPathologyPDL1.cpp
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//

#include <memory>
#include "DataObj.h"
#include "InferLocalPathologyPDL1.h"
#include <vector>

namespace ALPHA
{
	namespace PDL1
	{
		InferLocalPathologyPDL1::InferLocalPathologyPDL1() : COMM::InferLocalBase()
		{

		}

		InferLocalPathologyPDL1::~InferLocalPathologyPDL1()
		{

		}
	//**********************************************************************************************************************
		cv::Mat InferLocalPathologyPDL1::resize_image(cv::Mat in_img, const int input_width, const int input_height)
		{
			cv::Size dsize = cv::Size(in_img.cols, in_img.rows);
			cv::Mat out_img = cv::Mat(dsize, CV_8UC3);
			cv::resize(in_img, out_img, cv::Size(input_width, input_height));
			return out_img;
		}

		//**********************************************************************************************************************
		bool InferLocalPathologyPDL1::Mat2ChannelFirstTensor(cv::Mat in_img, tensorflow::Tensor& out_tensor, const int batch)
		{
			auto out_tensorMapped = out_tensor.tensor<float, 4>();
			int height = in_img.rows;
			int width = in_img.cols;
			int depth = in_img.channels();
			const float* data = (float*)in_img.data;
			for (int y = 0; y < height; y++)
			{
				const float* dataRow = data + (y * width * depth);
				for (int x = 0; x < width; x++)
				{
					const float* dataPixel = dataRow + (x * depth);
					for (int c = 0; c < depth; c++)
					{
						const float* dataValue = dataPixel + c;
						out_tensorMapped(batch, c, y, x) = *dataValue;
					}
				}
			}
			return true;
		}
		//**********************************************************************************************************************
		cv::Mat InferLocalPathologyPDL1::vector2Mat(std::vector<std::vector<std::vector<uchar>>> input, int channel) {
			cv::Mat ret(input.size(), input[0].size(), CV_8UC1, cv::Scalar::all(0));
			for (int i = 0; i < input.size(); i++) {
				uchar* data_mat = ret.ptr<uchar>(i);
				for (int j = 0; j < input[0].size(); j++) {
					data_mat[j] = input[i][j][channel];
				}
			}
			return ret;
		}
		//**********************************************************************************************************************
		std::vector<std::vector<std::pair<int, int>>> InferLocalPathologyPDL1::FindContour(cv::Mat input) {
			std::vector<std::vector<cv::Point>> contours;
			std::vector<std::vector<std::pair<int, int>>> ret;

			/*cv::imshow("draw", input);
			cv::waitKey();*/
			
			cv::findContours(input, contours, 0, 1);
			for (int num = 0; num < contours.size(); num++) {
				std::vector<std::pair<int, int>> points;
				for (int num_points = 0; num_points < contours[num].size(); num_points++) {
					cv::Point p = contours[num][num_points];
					int x = p.x;
					int y = p.y;
					std::pair<int, int> point_temp(x, y);
					points.push_back(point_temp);
				}
				ret.push_back(points);
			}
			return ret;
		}
		//**********************************************************************************************************************
		std::vector<int> InferLocalPathologyPDL1::cal_sum(std::vector<std::vector<std::vector<uchar>>> seg_reshape) {
			int num_classes = seg_reshape[0][0].size();
			int height = seg_reshape.size();
			int width = seg_reshape[0].size();
			std::vector<int> ret(num_classes, 0);

			for (int c = 0; c < num_classes; c++) {
				int summ = 0;
				for (int h = 0; h < height; h++) {
					for (int w = 0; w < width; w++) {
						summ += seg_reshape[h][w][c];
					}
				}
				ret[c] = summ;
			}
			return ret;
		}
		//**********************************************************************************************************************
		cv::Mat InferLocalPathologyPDL1::normalize_image(cv::Mat in_img)
		{
			int rows = in_img.rows;
			int cols = in_img.cols;

			//create a float Mat to store the pixel intensity
			cv::Mat norm_img = cv::Mat(rows, cols, CV_32FC3, cv::Scalar::all(0));
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					cv::Vec3b pixel = in_img.at<cv::Vec3b>(i, j);
					uchar red = pixel.val[0];
					uchar green = pixel.val[1];
					uchar blue = pixel.val[2];

					float replace_b = blue / 255.0;
					float replace_g = green / 255.0;
					float replace_r = red / 255.0;

					float* r = &norm_img.at<cv::Vec3f>(i, j)[0];
					float* g = &norm_img.at<cv::Vec3f>(i, j)[1];
					float* b = &norm_img.at<cv::Vec3f>(i, j)[2];

					*b = replace_b;
					*g = replace_g;
					*r = replace_r;

				}
			}
			return norm_img;
		}
		//**********************************************************************************************************************
		bool InferLocalPathologyPDL1::Mat2ChannelLastTensor(cv::Mat in_img, tensorflow::Tensor& out_tensor, const int batch)
		{
			auto out_tensorMapped = out_tensor.tensor<float, 4>();
			int height = in_img.rows;
			int width = in_img.cols;
			int depth = in_img.channels();
			const float* data = (float*)in_img.data;
			for (int c = 0; c < depth; c++)
			{
				const float* dataRow = data + (c * width * height);
				for (int y = 0; y < height; y++)
				{
					const float* dataPixel = dataRow + (y * width);
					for (int x = 0; x < width; x++)
					{
						const float* dataValue = dataPixel + x;
						out_tensorMapped(batch, y, x, c) = *dataValue;
					}
				}
			}
			return true;
		}

	//**********************************************************************************************************************
		COMM::DataObj* InferLocalPathologyPDL1::predict(COMM::DataObj* dataObj, const std::string& modelName)
		{
			IFPipelinePathologyPDL1In* input_PDL1 = dynamic_cast<IFPipelinePathologyPDL1In*>(dataObj);
			libconfig::Config mConfig;
			COMM::loadCfg(input_PDL1->m_config_path, mConfig);

			const libconfig::Setting& root = mConfig.getRoot();
			const libconfig::Setting& shape = root["PDL1_PREPROCESS"]["INPUT_SHAPE"];

			int width = shape[0];
			int height = shape[1];

			float prob_threshold_cls = mConfig.lookup("PT_PDL1.SEG_THRESHOLD");
			int batch_size = mConfig.lookup("PT_PDL1.BATCH_SIZE");
			std::string input_node = mConfig.lookup("PT_PDL1.INPUT_NODE");
			std::string output_node = mConfig.lookup("PT_PDL1.OUTPUT_NODE");
			int num_classes = mConfig.lookup("PT_PDL1.NUM_CLASSES");

			IFPipelinePathologyPDL1Out* output_PDL1(new IFPipelinePathologyPDL1Out());

			
			// step 1: make prediction
			//**************************************************************
			int total_summ_0 = 0;
			int total_summ_1 = 0;
			for (std::string path : input_PDL1->m_image_list)
			{
				cv::Mat img;
				img = cv::imread(path);
				if (img.empty())
				{
					LOG4CPLUS_WARN(COMM::MyLogger::getInstance()->m_rootLog, "Could not open image: " << path);
					LOG4CPLUS_WARN(COMM::MyLogger::getInstance()->m_rootLog, "Is it broken? ");
					continue;
				}

				PatchInfo patch_info;
				patch_info.image_name = path.substr(path.find_last_of('\\') + 1, -1);
				patch_info.sample_name = input_PDL1->m_slide_name;
				cv::Mat input_img = this->resize_image(img, width, height);
				input_img = this->normalize_image(input_img);

				tensorflow::Tensor img_tensor_seg(tensorflow::DT_FLOAT, tensorflow::TensorShape({ batch_size, width, height, 3 }));
				float* p = img_tensor_seg.flat<float>().data();
				cv::Mat img_tensor_mat(width, height, CV_32FC3, p);
				input_img.convertTo(img_tensor_mat, img_tensor_mat.type());

				std::vector<tensorflow::Tensor> seg_res;
				std::pair<std::string, tensorflow::Tensor>img_feed(input_node, img_tensor_seg);

				const std::string trainable = "trainable:0";
				tensorflow::Tensor trainable_tensor(tensorflow::DT_BOOL, tensorflow::TensorShape());
				auto t_trainable = trainable_tensor.scalar<bool>();
				t_trainable(0) = false;
				std::pair<std::string, tensorflow::Tensor>img_feed_trainable(trainable, trainable_tensor);
				std::vector<std::pair<std::string, tensorflow::Tensor>> feed_dict = { {img_feed, img_feed_trainable} };

				tensorflow::Status status_1 = this->m_session_seg->Run(feed_dict, {output_node}, {}, &seg_res);

				
				tensorflow::Tensor t_seg = seg_res[0];
				auto seg_ = t_seg.flat<float>();
				
				std::vector<std::vector<std::vector<std::vector<uchar>>>> seg_reshape(batch_size, std::vector<std::vector<std::vector<uchar>>>(height, std::vector<std::vector<uchar>>(width, std::vector<uchar>(num_classes, 0))));
				
				//put value in a 4-dimension vector
				int idx = 0;
				for (unsigned int i = 0; i < batch_size; i++) {
					for (unsigned int j = 0; j < height; j++) {
						for (unsigned int k = 0; k < width; k++) {
							for (unsigned int l = 0; l < num_classes; l++) {
								float temp_fm = seg_(idx);
								if (l == 0) {
									if (temp_fm > prob_threshold_cls)
										seg_reshape[i][j][k][l] = 1;
								}
								else {
									if (temp_fm > prob_threshold_cls && seg_reshape[i][j][k][0] == 0)
										seg_reshape[i][j][k][l] = 1;
								}
								idx++;
							}
						}
					}
				}

				
				std::vector<int> summ_res = this->cal_sum(seg_reshape[0]);
				total_summ_0 += summ_res[0];
				total_summ_1 += summ_res[1];
				patch_info.ratio = float(summ_res[0])*1.0 / float(summ_res[1] + summ_res[0]);
				for (int c = 0; c < num_classes; c++) {
					cv::Mat temp = this->vector2Mat(seg_reshape[0], c);
					patch_info.contour.push_back(this->FindContour(temp));
				}
				output_PDL1->sample_details.push_back(patch_info);
			}
				LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "All images prediction done");
				if (output_PDL1->sample_details.empty()) {
					output_PDL1->all_image_broken = true;
					LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "None of the input image could be opened!");
				}

				output_PDL1->sample_name = input_PDL1->m_slide_name;
				float final_ratio = float(total_summ_0) / (total_summ_0 + total_summ_1 + 0.001);
				output_PDL1->tps_ratio = final_ratio;
			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Return results");
			return output_PDL1;
		}
	}
}