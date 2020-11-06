#if !defined(_IFPIPELINEPATHOLOGYIN_H)
#define _IFPIPELINEPATHOLOGYIN_H

#include <vector>
#include <string>
#include <array>
#include <io.h>

#include "IFBase.h"
#include "PipelinePathologyROSE_export.h"

namespace ALPHA
{
	namespace ROSE
	{
		class PIPELINEPATHOLOGYROSE_EXPORT IFPipelinePathologyIn : public COMM::IFBase {
		public:
			explicit IFPipelinePathologyIn();
			explicit IFPipelinePathologyIn(std::string& svs_path, std::string& config_path);
			~IFPipelinePathologyIn() override {}

			// according to the data structure of Pyxis
			std::string m_image_folder;
			std::string m_image_folder_path;
			std::string m_slide_name;
			std::string m_wsi_preview_path;
			std::string m_scan_info_path;
			std::string m_slide_preview_path;
			std::string m_config_path;
			std::array<std::string, 3> m_support_format = { ".jpg", ".tif", ".png" };
			std::vector<std::string> m_image_list;
			std::string m_image_suffix;
			int m_num_image = 0;

			//svs
			std::array<std::string, 4> m_svs_support_format = { ".tif", ".tiff", ".svs" , ".ndpi"};
			std::string m_svs_path;
			bool m_format_dismatch = true;
		};
	}
}



#endif  //_IFPIPELINEPATHOLOGYIN_H