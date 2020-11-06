#pragma once

#include <string>
#include "IFBase.h"
#include <vector>
#include <array>
#include <io.h>
#include "IFBase.h"
#include "PipelinePathologyXS_export.h"
namespace ALPHA
{
	namespace XS
	{

		class PIPELINEPATHOLOGYXS_EXPORT IFPipelinePathologyXSIn : public COMM::IFBase
		{
		public:
			explicit IFPipelinePathologyXSIn();
			explicit IFPipelinePathologyXSIn(std::string& svs_path, std::string& config_path);
			~IFPipelinePathologyXSIn() override {}
			
			std::string m_svs_path;
			std::string m_image_folder;
			bool control = true;
			bool algorithm = true;
			std::string m_slide_name;
			std::string m_wsi_preview_path;
			//libconfig::Config mConfig;
			std::string m_config_path;
			std::string m_scan_info_path;
			std::string m_slide_preview_path;
			bool m_format_dismatch = true;

			std::vector<std::string> m_image_list;
			std::array<std::string, 3> m_support_format = { ".jpg", ".tif", ".png" };
			std::array<std::string, 4> m_svs_support_format = { ".tif", ".tiff", ".svs" , ".ndpi" };
			std::string m_image_suffix;
			int m_num_images = 0;
			int num_tumor_patch = 0;
		};


	};
};
