#pragma once

#include <string>
#include <iostream>
#include "IFBase.h"
#include <vector>
#include <array>
#include <io.h>
#include "IFBase.h"
#include "PipelinePathologyTCT_export.h"
namespace ALPHA
{
	namespace TCT
	{

		class PIPELINEPATHOLOGYTCT_EXPORT IFPipelinePathologyTCTIn : public COMM::IFBase
		{
		public:
			explicit IFPipelinePathologyTCTIn();
			explicit IFPipelinePathologyTCTIn(std::string& image_folder, std::string& config_path, bool qc, bool alg);
			~IFPipelinePathologyTCTIn() override {}
			std::string m_svs_path = "";
			std::string m_image_folder;
			bool control = true;
			bool algorithm = true;
			std::string m_slide_name;
			std::string m_wsi_preview_path;
			//libconfig::Config mConfig;
			std::string m_config_path;
			std::string m_scan_info_path;
			std::string m_slide_preview_path;
			
			std::vector<std::string> m_image_list;
			std::array<std::string, 3> m_support_format = { ".svs", ".tiff", ".tif" };
			std::string m_image_suffix;
			int m_num_images = 0;
			int num_tumor_patch = 0;
		};


	};
};

