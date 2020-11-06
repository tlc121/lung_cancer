//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : TCT
//  @ File Name : InferLocalPathologyTCT.cpp
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//

#include <memory>
#include "DataObj.h"
#include "InferLocalPathologyTCT.h"
#include <vector>
#include <map>
#include <stdlib.h>

namespace ALPHA
{
	namespace TCT
	{
		InferLocalPathologyTCT::InferLocalPathologyTCT() : COMM::InferLocalBase()
		{

		}

		InferLocalPathologyTCT::~InferLocalPathologyTCT()
		{

		}

	//**********************************************************************************************************************
		COMM::DataObj* InferLocalPathologyTCT::predict(COMM::DataObj* dataObj, const std::string& modelName)
		{
			IFPipelinePathologyTCTIn* input_TCT = dynamic_cast<IFPipelinePathologyTCTIn*>(dataObj);

			int cut_width = 645;
			int cut_height = 608;

			bool algorithm = input_TCT->algorithm;
			bool quality_control = input_TCT->control;


			IFPipelinePathologyTCTOut* output_TCT(new IFPipelinePathologyTCTOut());

			// step 1: compute cell-ratio
			std::string preview_path;
			preview_path = input_TCT->m_wsi_preview_path;
			cv::Mat preview_img;
			preview_img = cv::imread(preview_path);
			if (preview_img.empty()) {
				LOG4CPLUS_WARN(COMM::MyLogger::getInstance()->m_rootLog, "Could not open image: " << preview_path);
				LOG4CPLUS_WARN(COMM::MyLogger::getInstance()->m_rootLog, "Is it broken or it doesn't exist?");
			}
			else {
				cv::Mat preview_gray;
				cv::cvtColor(preview_img, preview_gray, cv::COLOR_BGR2GRAY);
				cv::Mat dstgrayImage;
				cv::threshold(preview_gray, dstgrayImage, 0, 1, cv::THRESH_OTSU);

				cv::Scalar cell_num = sum(dstgrayImage);
				double rows = (double)dstgrayImage.rows;
				double cols = (double)dstgrayImage.cols;

				output_TCT->cell_ratio = floor((rows * cols - cell_num.val[0]) / 100) * 2;

				std::cout << "cell calculation done" << std::endl;
			}


			std::string format = input_TCT->m_svs_path.substr(input_TCT->m_svs_path.find_last_of('.'), -1);
			if (format == ".svs" || format == ".SVS" || format == ".tiff" || format == ".tif")
				output_TCT->is_empty = false;
			else {
				output_TCT->is_empty = true;
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, " Image Format is not right ");
				return output_TCT;
			}


			//Step2: read svs image & get dimension of it
			const char* filename = input_TCT->m_svs_path.c_str();
			if (!openslide_can_open(filename)) {
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, " Image is broken ");
				output_TCT->all_image_broken = true;
				return output_TCT;
			}


			openslide_t* slide = openslide_open(filename);
			if (openslide_get_error(slide) != NULL)
			{
				openslide_close(slide);
				std::cout << "slide corruption" << std::endl;
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, " Is it broken? ");
				output_TCT->all_image_broken = true;
				return output_TCT;
			}
			else {
				LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, " svs loading success ");
			}

			int64_t w_slide = 0;
			int64_t h_slide = 0;
			openslide_get_level_dimensions(slide, 0,  &w_slide, &h_slide);
			std::cout << "width & heigth is: " << w_slide << "&" << h_slide << std::endl;
			//Step3: calculate how many patches could be cut from slide
			int width_num = (w_slide) / cut_width;
			int height_num = (h_slide) / cut_height;
			input_TCT->m_num_images = width_num * height_num;
			int total_imgs = input_TCT->m_num_images;

			// step 4: make prediction
			//**************************************************************
			int image_with_cells = 0; //num of patches with cells
			int blur_images = 0; //num of patches which is blurry
			int zazhi_images = 0;//num of patches which is dirty
			int image_idx = 0;
			float intensity_scale = 1.0;
			int patch_index = 0;
			int batch_index_cls = 0;
			//std::list<patch> m_patchList;
			std::cout << "num_images: " << total_imgs << std::endl;
			IoThread* io_1 (new IoThread(slide, cut_width, cut_height, 0, 0));
			IoThread* io_2 (new IoThread(slide, cut_width, cut_height, 1, 0));
			IoThread* io_3 (new IoThread(slide, cut_width, cut_height, 0, 1));
			IoThread* io_4 (new IoThread(slide, cut_width, cut_height, 1, 1));
			AIThread* ai (new AIThread(output_TCT, input_TCT, this->m_session_qc, this->m_session_cls, this->m_session_det, io_1));
			//AIThread* ai_2(new AIThread(output_TCT, input_TCT, this->m_session_qc, this->m_session_cls, this->m_session_det, io_1));
			io_1->startThread();
			io_2->startThread();
			io_3->startThread();
			io_4->startThread();
			ai->startThread();
			io_1->m_run_thread->join();
			io_2->m_run_thread->join();
			io_3->m_run_thread->join();
			io_4->m_run_thread->join();
			std::cout << "prediction done" << std::endl;

			openslide_close(slide);
			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Return results");
			return output_TCT;
		}
	}
}