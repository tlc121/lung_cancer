#include <iostream>
#include <memory>
#include "InferLocalPathologyUtils.h"
#include "IFPipelinePathologyUtilsIn.h"
#include "IFPipelinePathologyUtilsOut.h"
#include "PipelinePathologyUtils.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include <time.h>





int main()
{
	std::cout << "-------test------" << std::endl;
	std::string image = "D:\\data\\preview¹þ¹þ\\Preview-2019-10-25-142911#4.JPG";
    std::string config_path = "D:\\tlc\\4.11\\alpha\\configuration\\pathology_config.cfg";

	//std::shared_ptr<ALPHA::Utils::PipelinePathologyUtilsFactory> ptrFactory(new ALPHA::Utils::PipelinePathologyUtilsFactory());
	std::shared_ptr<ALPHA::Pathology::PTUtils::IFPipelinePathologyUtilsIn> ptrPipelineInput(new ALPHA::Pathology::PTUtils::IFPipelinePathologyUtilsIn());
	ptrPipelineInput->set_path(image, config_path);
	std::shared_ptr<ALPHA::Pathology::PTUtils::PipelinePathologyUtils> pipeline(new ALPHA::Pathology::PTUtils::PipelinePathologyUtils(config_path, "TCT"));
	//std::shared_ptr<ALPHA::COMM::PipelineBase> pipeline = ptrFactory->createPipeline();
	pipeline->read_tct_model();
	pipeline->read_rose_model();
	
	pipeline->setInput(ptrPipelineInput);
	std::cout << "input done" << std::endl;
	//clock_t t_read_model_start = clock();
	pipeline->update();
	//std::cout << "alg takes: " <<(clock() - t_read_model_start) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
	std::shared_ptr<ALPHA::COMM::DataObj> ptroutput = pipeline->getOutput();
	std::shared_ptr<ALPHA::Pathology::PTUtils::IFPipelinePathologyUtilsOut> poutput = std::dynamic_pointer_cast<ALPHA::Pathology::PTUtils::IFPipelinePathologyUtilsOut>(ptroutput);
	std::cout << poutput->bbox[0] << std::endl;
	std::cout << poutput->bbox[1] << std::endl;
	std::cout << poutput->bbox[2] << std::endl;
	std::cout << poutput->bbox[3] << std::endl;
	int xmin = poutput->bbox[0];
	int ymin = poutput->bbox[1];
	int xmax = poutput->bbox[2];
	int ymax = poutput->bbox[3];
	cv::Mat ori_image;
	ori_image = cv::imread(image);
	cv::rectangle(ori_image, cv::Point(xmin, ymin), cv::Point(xmax, ymax), cv::Scalar(255, 0, 0), 4, 1, 0);
	cv::imshow("matImage", ori_image);
	cv::waitKey();




	//ALPHA::Utils::InferLocalPathologyUtils test;

	//test.predict(ptrPipelineInput.get(), "");

	//std::string img_path = "D:\\data\\ocr_20191121\\upload_03aadd8e9af549789a4579b6aa118005.png"; 
	//ALPHA::COMM::Pathoutils utils = ALPHA::COMM::Pathoutils(img_path);
	////int ret = utils.getArea();
	//std::string ret2 = utils.ocr();
	//std::cout << ret2 << std::endl;
	return 0;
}