//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Virus
//  @ File Name : PipelineElementPathologyVirusCls.cpp
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//

#include "PipelineElementPathologyVirusCls.h"

namespace ALPHA
{
	namespace VIRUS
	{
		PipelineElementPathologyVirusCls::PipelineElementPathologyVirusCls():
			COMM::PipelineElementAlgorithmMachineLearningBase()	{}

		PipelineElementPathologyVirusCls::~PipelineElementPathologyVirusCls() {}

		void PipelineElementPathologyVirusCls::setInput(std::shared_ptr< COMM::DataObj > Input)
		{
			ptrInterfaceData = std::dynamic_pointer_cast<IFPipelinePathologyIn>(Input);
		}

		std::shared_ptr<COMM::DataObj> PipelineElementPathologyVirusCls::getOutput() {

			//std::shared_ptr<COMM::DataObj> p(detectionOut);
			//return p;

			return detectionOut;
	
		}

		void PipelineElementPathologyVirusCls::preproc() {
			// init ouput 
			detectionOut = std::make_shared<IFPipelinePathologyVirusOut>();
			return;
		}

		void PipelineElementPathologyVirusCls::infer() {
		
			InferLocalPathologyVirus virusInfer;
			// infer.predict
			ptrInterfaceDataOut = virusInfer.predict(ptrInterfaceData.get(), "");

			std::shared_ptr<COMM::DataObj> p(ptrInterfaceDataOut);
			//virusOut = std::dynamic_pointer_cast<IFPipelinePathologyVirusOut>(p);
			virusOut = std::dynamic_pointer_cast<IFPipelinePathologyVirusTemp>(p);

		}

		void PipelineElementPathologyVirusCls::postproc() {

			detectionOut->sample_name = virusOut->sample_name;
			if (virusOut->ALL_IMAGE_BROKEN)
			{
                 detectionOut->ALL_IMAGE_BROKEN = virusOut->ALL_IMAGE_BROKEN;
				 return;
			}
			   
			tensor2Det(virusOut->TARGET_CONF_THRESH);
			LOG4CPLUS_DEBUG(COMM::MyLogger::getInstance()->m_rootLog, "Convert Tensor to Detection Done!");
			//std::cout << "convert to preditions" << std::endl;
			// iter per image

			for (int64_t i = 0; i < detectionOut->preditions.size(); i++) {
				detectionOut->image_names.push_back(virusOut->image_names[i]);
				std::vector<Detection>& bboxes = detectionOut->preditions[i];

				if (bboxes.empty()) {
					// image did not have any bboxes
					detectionOut->image_results.push_back(0);
					detectionOut->image_type.push_back("Normal");
					continue;
				}
				detectionOut->image_results.push_back(bboxes.size() > virusOut->TARGET_NUM_THRESH ? 1 : 0);
				detectionOut->image_type.push_back(bboxes.size() > virusOut->TARGET_NUM_THRESH ? "Virus" : "Normal");
			
				for (Detection &bbox: bboxes)
				{
					bbox.left = std::round(std::max((bbox.left * virusOut->ori_img_shapes[i].second / virusOut->IMAGE_SIZE),0));
					bbox.top = std::round(std::max((bbox.top * virusOut->ori_img_shapes[i].first / virusOut->IMAGE_SIZE), 0));
					bbox.right = std::round(std::min((bbox.right * virusOut->ori_img_shapes[i].second / virusOut->IMAGE_SIZE), virusOut->ori_img_shapes[i].second-1));
					bbox.bottom = std::round(std::min((bbox.bottom * virusOut->ori_img_shapes[i].first / virusOut->IMAGE_SIZE), virusOut->ori_img_shapes[i].first-1));
				}

				if (bboxes.size() == 1) {
					Detection& bbox = bboxes[0];
					cv::Mat ori_image = cv::imread(ptrInterfaceData->m_image_list[i], cv::IMREAD_COLOR);
					cv::Mat pred_ROI = ori_image(cv::Rect2i(bbox.left, bbox.top, bbox.right - bbox.left, bbox.bottom - bbox.top));
					//pred_ROI is BGR image
					auto main_color = postProcessor.get_color(postProcessor.rm_bg(pred_ROI));
					// remove false positive				
					if (main_color != "green") {	
						LOG4CPLUS_DEBUG(COMM::MyLogger::getInstance()->m_rootLog, "Image "<< virusOut->image_names[i]<< " -> main color: " << main_color);
						LOG4CPLUS_DEBUG(COMM::MyLogger::getInstance()->m_rootLog, "Remove false positive box");
						bboxes.pop_back();
						detectionOut->image_results.pop_back();
						detectionOut->image_results.push_back(0);
						detectionOut->image_type.pop_back();
						detectionOut->image_type.push_back("Normal");
					}					
				} 
			}

			int sample_result = 0;
			for (auto & cnt: detectionOut->image_results) 			
				sample_result += cnt;

			detectionOut->sample_result = sample_result;
			//saveJson();

			return;
		}


		void PipelineElementPathologyVirusCls::tensor2Det(float conf_thresh) {
			// outputs -> 2d tenor -> [num_obj, bboxes]
            // bboxes: [class_index, class_prob, topleft_x, topleft_y, bottomright_x, bottomright_y]
			std::vector<tensorflow::Tensor>& outputs = virusOut->outputs;
			for (int64_t i = 0; i < outputs.size(); i++)
			{
				tensorflow::Tensor temp = outputs[i];
				int stride = temp.dim_size(2);
				std::vector<Detection> preds;

				// the output[0] is a 3d tensor: [1, 200, 6]. flat to 1d
				const Eigen::TensorMap<Eigen::Tensor<float, 1, Eigen::RowMajor>, Eigen::Aligned>& m = temp.flat<float>();

				long j = 0;
				while (j < m.size()){
					int cls = static_cast<int>(m(j));
					float conf = m(j + 1);

					if ((cls != 0) && (conf > conf_thresh ))
					{
						Detection det = {static_cast<int>(m(j)),  static_cast<float>(m(j + 1)),
										 static_cast<int>(m(j + 2)), static_cast<int>(m(j + 3)),
										 static_cast<int>(m(j + 4)), static_cast<int>(m(j + 5)) };
						preds.emplace_back(det);
					}
					j += stride;
				}

				//covert to 2d tensor to EigenMat
				//auto m = Eigen::Map<Eigen::Matrix<
				//	float,           /* scalar element type */
				//	Eigen::Dynamic,  /* num_rows is a run-time value */
				//	Eigen::Dynamic,  /* num_cols is a run-time value */
				//	Eigen::RowMajor  /* tensorflow::Tensor is always row-major */>>(
				//		temp.flat<float>().data(),  /* ptr to data */
				//		temp.dim_size(0),           /* num_rows */
				//		temp.dim_size(1)            /* num_cols */);

				//Eigen matrix ops
				//convert to detection	
				//for (int64_t j = 0; j < temp.dim_size(0); j++) {

				//	float conf = static_cast<float>(m.row(j)[1]);
				//	float cls = static_cast<int>(m.row(j)[0]);
				//	if ((cls != 0) && (conf > conf_thresh ))
				//	{
				//		Detection det = { static_cast<int>(m.row(j)[0]),  static_cast<float>(m.row(j)[1]),
				//						  static_cast<int>(m.row(j)[2]), static_cast<int>(m.row(j)[3]),
				//						  static_cast<int>(m.row(j)[4]), static_cast<int>(m.row(j)[5]) };
				//		preds.emplace_back(det);
				//	}
				//}		
				detectionOut->preditions.push_back(preds);
			}
		}

		//void PipelineElementPathologyVirusCls::saveJson(float target_conf_the, float target_num_the) {

		//	for (int64_t i = 0; i < detectionOut->preditions.size(); i++) {
		//		std::vector<Detection>& dets = detectionOut->preditions[i];
		//		nlohmann::json img_info;
		//		img_info["img_name"] = detectionOut->image_name[i];
		//		img_info["sample_name"] = detectionOut->sample_name;
		//		img_info["image_results"] = dets.size() > target_num_the ? 1 : 0;
		//		
		//		nlohmann::json img_details = nlohmann::json::array();
		//		if (!dets.empty()) {
		//			for (auto& bbox : dets)
		//			{
		//				nlohmann::json dj;
		//				//Transform the predicted bounding boxes for the 512x512 image to the original image dimensions.
		//				dj = nlohmann::json{ {"type", bbox.cls}, {"prob",bbox.conf},
		//									 {"topleft_x", bbox.left}, 
		//				                     {"topleft_y", bbox.top},
		//									 {"bottomright_x", bbox.right},
		//				                     {"bottomright_y", bbox.bottom}
		//				                    };
		//				std::cout << "JSON: "<< dj << std::endl;
		//				img_details.push_back(dj);
		//			}
		//		}
		//		img_info["img_details"] = img_details;
		//		//virusOut->results.push_back(img_info);
		//	}
		//}
		//void  IFPipelinePathologyVirusOut::dumpJson() {
		//	for (int64_t i = 0; i < results.size(); i++) {
		//		std::string filename = outdir + "\\" + sample_details[i].image_name + ".json";
		//		std::ofstream fs(filename);
		//		if (!fs)
		//			std::cerr << "Cannot open the output file." << std::endl;
		//		fs << results[i].dump();
		//		fs.close();
		//	}
		//}
	}
}

