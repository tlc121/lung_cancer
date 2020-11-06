#pragma once

#include <string>
#include "IFBase.h"
#include <vector>
#include <array>
#include <io.h>
#include "IFBase.h"
#include "PipelinePathologyPDL1_export.h"
namespace ALPHA
{
	namespace PDL1
	{

		class PIPELINEPATHOLOGYPDL1_EXPORT IFPipelinePathologyPDL1In : public COMM::IFBase
		{
		public:
			explicit IFPipelinePathologyPDL1In();
			explicit IFPipelinePathologyPDL1In(std::string& image_folder, std::string& config_path);
			~IFPipelinePathologyPDL1In() override {}
			
			std::string m_image_folder;
			std::string m_slide_name;
			std::string m_wsi_preview_path;
			std::string m_config_path;
			std::string m_scan_info_path;
			std::string m_slide_preview_path;
			
			std::vector<std::string> m_image_list;
			std::array<std::string, 3> m_support_format = { ".jpg", ".tif", ".png" };
			std::string m_image_suffix;
			int m_num_images = 0;
			int num_tumor_patch = 0;
		};


	};
};

