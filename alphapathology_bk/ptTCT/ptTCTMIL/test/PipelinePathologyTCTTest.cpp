#include <iostream>
#include <memory>
#include "InferLocalPathologyTCT.h"
#include "IFPipelinePathologyTCTIn.h"
#include "IFPipelinePathologyTCTOut.h"
#include "PipelinePathologyTCT.h"
#include <time.h>


cv::Mat read_patch(openslide_t* input, int x_start, int y_start, int width, int height, int level) {

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



int main()
{
		
		std::cout << "-------test------" << std::endl;
		std::string image_folder = "D:\\data\\tif\\b1aac418cb14094307bbb1a36fdf92bf475721bc.svs";
		std::string config_path = "D:\\tlc\\TCT\\4.44\\alpha_4thread\\configuration\\pathology_config.cfg";
		bool qc = true;
		bool alg = true;
		//std::shared_ptr<ALPHA::TCT::PipelinePathologyTCTFactory> ptrFactory(new ALPHA::TCT::PipelinePathologyTCTFactory());
		openslide_t* slide = openslide_open(image_folder.c_str());
		std::shared_ptr<ALPHA::TCT::IFPipelinePathologyTCTIn> ptrPipelineInput(new ALPHA::TCT::IFPipelinePathologyTCTIn(image_folder, config_path, qc, alg));
		std::shared_ptr<ALPHA::TCT::PipelinePathologyTCT> pipeline(new ALPHA::TCT::PipelinePathologyTCT(config_path));
		//std::shared_ptr<ALPHA::COMM::PipelineBase> pipeline = ptrFactory->createPipeline();
		if (alg) {
			pipeline->read_cls_model();
			pipeline->read_det_model();
		}
		if (qc)
			pipeline->read_qc_model();

		pipeline->setInput(ptrPipelineInput);
		clock_t t_update_start = clock();
		pipeline->update();
		std::cout << "get output" << std::endl;
		std::cout << "alg takes: " << (clock() - t_update_start) * 1.0 / CLOCKS_PER_SEC << "s" << std::endl;
		std::cout << "get output" << std::endl;
		std::shared_ptr<ALPHA::COMM::DataObj> ptroutput = pipeline->getOutput();
		std::shared_ptr<ALPHA::TCT::IFPipelinePathologyTCTOut> poutput = std::dynamic_pointer_cast<ALPHA::TCT::IFPipelinePathologyTCTOut>(ptroutput);
		std::cout << "start printing result: " << std::endl;
		std::cout << "name: " << poutput->sample_name << std::endl;
		std::cout << "result: " << poutput->sample_result << std::endl;
		std::cout << "disease type: " << poutput->disease_type << std::endl;
		std::cout << "blur ratio: " << poutput->blur_ratio << std::endl;
		std::cout << "zazhi ratio: " << poutput->zazhi_ratio << std::endl;
		std::cout << "blur image num: " << poutput->blur_images << std::endl;
		std::cout << "zazhi image num: " << poutput->zazhi_images << std::endl;
		std::cout << "image with cell: " << poutput->image_with_cells << std::endl;
		std::cout << "num_bboxes: " << poutput->num_bboxes << std::endl;
		std::cout << "num_patch: " << ptrPipelineInput->num_tumor_patch << std::endl;
	
	for (auto bbox : poutput->bboxes_res) {
		std::cout << "xmin: " << bbox.first[0] << std::endl;
		std::cout << "ymin: " << bbox.first[1] << std::endl;
		std::cout << "xmax: " << bbox.first[2] << std::endl;
		std::cout << "ymax: " << bbox.first[3] << std::endl;
		std::cout << "prob: " << bbox.second << std::endl;
		cv::Mat img = read_patch(slide, bbox.first[0], bbox.first[1], bbox.first[2]- bbox.first[0], bbox.first[3] - bbox.first[1], 0);
		cv::imshow("sd", img);
		cv::waitKey(0);

	}


	//ALPHA::TCT::InferLocalPathologyTCT test;

	//test.predict(ptrPipelineInput.get(), "");

	//std::string img_path = "D:\\data\\ocr_20191121\\upload_03aadd8e9af549789a4579b6aa118005.png"; 
	//ALPHA::COMM::Pathoutils utils = ALPHA::COMM::Pathoutils(img_path);
	////int ret = utils.getArea();
	//std::string ret2 = utils.ocr();
	//std::cout << ret2 << std::endl;
	return 0;
}