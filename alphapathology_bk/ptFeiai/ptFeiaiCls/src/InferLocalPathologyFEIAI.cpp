//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : FEIAI
//  @ File Name : InferLocalPathologyFEIAI.cpp
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//

#include <memory>
#include "DataObj.h"
#include "InferLocalPathologyFEIAI.h"
#include <vector>

namespace ALPHA
{
	namespace FEIAI
	{
		InferLocalPathologyFEIAI::InferLocalPathologyFEIAI() : COMM::InferLocalBase()
		{

		}

		InferLocalPathologyFEIAI::~InferLocalPathologyFEIAI()
		{

		}

		

	//**********************************************************************************************************************
		COMM::DataObj* InferLocalPathologyFEIAI::predict(COMM::DataObj* dataObj, const std::string& modelName)
		{
			IFPipelinePathologyFEIAIIn* input_FEIAI = dynamic_cast<IFPipelinePathologyFEIAIIn*>(dataObj);
			IFPipelinePathologyFEIAIOut* output_feiai(new IFPipelinePathologyFEIAIOut());
			int cut_width = 1024;
			int cut_height = 1024;
			std::string config_path =  input_FEIAI->m_config_path;
			if (input_FEIAI->m_format_dismatch)
			{
				output_feiai->format_wrong = true;
				return output_feiai;
			}
			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Start reading svs: " << input_FEIAI->m_svs_path.c_str());

			if (!openslide_can_open(input_FEIAI->m_svs_path.c_str())) {
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, " Image is broken ");
				output_feiai->file_broken = true;
				return output_feiai;
			}


			openslide_t* slide = openslide_open(input_FEIAI->m_svs_path.c_str());
			if (openslide_get_error(slide) != NULL)
			{
				openslide_close(slide);
				std::cout << "slide corruption" << std::endl;
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "Could not read slide.");
				output_feiai->file_broken = true;
				return output_feiai;
			}
			else
			{
				LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Load slide success.");
			}
			ALPHA::FEIAI::IOThread io(input_FEIAI, slide, cut_width, cut_height);
			ALPHA::FEIAI::AIThread ai(config_path, output_feiai, &io);
			io.startThread();
			ai.startThread();
			io.m_run_thread->join();
			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Return results");
			return output_feiai;
		}
	}
}