//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Virus
//  @ File Name : InferLocalPathologyVirus.cpp
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//

#include <memory>

#include "DataObj.h"
#include "InferLocalPathologyVirus.h"
#include "IFPipelinePathologyVirusIn.h"


namespace ALPHA
{
	namespace VIRUS
	{
		InferLocalPathologyVirus::InferLocalPathologyVirus() : COMM::InferLocalBase() {}
		InferLocalPathologyVirus::~InferLocalPathologyVirus() {}

		//void InferLocalPathologyVirus::tensor2Mat(tensorflow::Tensor& t, cv::Mat& image) {
		//	int* p = t.flat<int>().data();
		//	image = cv::Mat(IMAGE_SIZE, IMAGE_SIZE, CV_32SC1, p);
		//	image.convertTo(image, CV_8UC1);
		//}

		// read, resize, toTensor
		void InferLocalPathologyVirus::mat2Tensor(cv::Mat image, tensorflow::Tensor& t) {
			float* tensor_data_ptr = t.flat<float>().data();
			cv::Mat fake_mat(image.rows, image.cols, CV_32FC(image.channels()), tensor_data_ptr);
			image.convertTo(fake_mat, CV_32FC(image.channels()));
		}


		COMM::DataObj* InferLocalPathologyVirus::predict(COMM::DataObj* dataObj, const std::string& modelName)
		{

			IFPipelinePathologyIn* ptrDataInput = dynamic_cast<IFPipelinePathologyIn*>(dataObj);
			//IFPipelinePathologyVirusOut* virusOutput(new IFPipelinePathologyVirusOut());
			IFPipelinePathologyVirusTemp* virusOutput(new IFPipelinePathologyVirusTemp());
			//load config file
			libconfig::Config mConfig;
			COMM::loadCfg(ptrDataInput->m_config_path, mConfig);

			const int IMAGE_SIZE = mConfig.lookup("VIRUS_DET.IMAGE_SIZE");
			std::string input_node_name = mConfig.lookup("VIRUS_DET.INPUT_NODE");
			std::string output_node_name = mConfig.lookup("VIRUS_DET.OUTPUT_NODE");
			std::string model_path = mConfig.lookup("VIRUS_DET.MODEL_PATH");
			virusOutput->IMAGE_SIZE = IMAGE_SIZE;
			virusOutput->TARGET_CONF_THRESH = mConfig.lookup("VIRUS_DET.TARGET_CONF_THRESH");
			virusOutput->TARGET_NUM_THRESH = mConfig.lookup("VIRUS_DET.TARGET_NUM_THRESH");

			tensorflow::GraphDef graphdef;
			//TF_CHECK_OK(ReadBinaryProto(Env::Default(), model_path, &graphdef));
			tensorflow::Status ret = tensorflow::ReadBinaryProto(tensorflow::Env::Default(), model_path, &graphdef);
			if (!ret.ok())
			{
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "Fail to load pb model! Please check model.pb exists!");
				throw std::invalid_argument("Fail to load pb model! Please check model.pb exists!");				
			}

			std::unique_ptr<tensorflow::Session> session;
			tensorflow::SessionOptions options;
			// Set GPU options
			//options.config.mutable_gpu_options()->set_visible_device_list(gpus);
			//options.config.allow_soft_placement();
			options.config.mutable_gpu_options()->set_allow_growth(true);
			(&session)->reset(NewSession(options));
			//TF_CHECK_OK(NewSession(options, &session));

			//TF_CHECK_OK(session->Create(graphdef)); // model to session
			tensorflow::Status ret2 = session->Create(graphdef);
			if (!ret2.ok())
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "Fail to create graph!");

			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Successfully created session and load graph.");

			for (int i = 0; i < ptrDataInput->m_num_image; i++) {

				std::string& img = ptrDataInput->m_image_list[i];
				cv::Mat ori_image = cv::imread(img, cv::IMREAD_COLOR);
				if (ori_image.empty())
				{
					LOG4CPLUS_WARN(COMM::MyLogger::getInstance()->m_rootLog, "Could not open image: " << img);
					LOG4CPLUS_WARN(COMM::MyLogger::getInstance()->m_rootLog, "Is it broken?");
					continue;
				}

				// record shape
				// keep tracks of image for post processing
				//virusOutput->images.push_back(ori_image.clone());
				virusOutput->ori_img_shapes.push_back(std::pair<int, int>(ori_image.rows, ori_image.cols));
				//resize 
				cv::resize(ori_image, ori_image, cv::Size(IMAGE_SIZE, IMAGE_SIZE), 0, 0, cv::INTER_LINEAR);  //cv::INTER_CUBIC ?
				// to RGB
				cvtColor(ori_image, ori_image, cv::COLOR_BGR2RGB);						
				//input tensor
				tensorflow::Tensor input(tensorflow::DT_FLOAT, tensorflow::TensorShape({ 1, IMAGE_SIZE, IMAGE_SIZE, 3 }));			
				//convert to tensor
				mat2Tensor(ori_image, input);
				// input and output
				std::vector<std::pair<std::string, tensorflow::Tensor>> inp = { {input_node_name, input} };
				std::vector<tensorflow::Tensor> output;
				// run infer
				tensorflow::Status status = session->Run(inp, { output_node_name }, {}, &output);
				if (!status.ok())
					LOG4CPLUS_DEBUG(COMM::MyLogger::getInstance()->m_rootLog, "Inference failed on image: " <<img);
				// check the dims of output[0] !!!
				// remove first dim
				//virusOutput->outputs.push_back(output[0].SubSlice(0));

				virusOutput->outputs.push_back(output[0]);
				//PatchInfo patchInfo;
				std::string imgname = img.substr(img.find_last_of('\\') + 1, -1);
				virusOutput->sample_name = ptrDataInput->m_slide_name;
				virusOutput->image_names.push_back(imgname);

			}

			if (virusOutput->outputs.empty()) {
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "None of the input image could be opened!");
				virusOutput->ALL_IMAGE_BROKEN = true;
			}
			return virusOutput;		
		}

	}
}