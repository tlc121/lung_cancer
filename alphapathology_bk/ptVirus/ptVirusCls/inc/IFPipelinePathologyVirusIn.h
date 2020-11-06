#if !defined(_IFPIPELINEPATHOLOGYIN_H)
#define _IFPIPELINEPATHOLOGYIN_H

#include <vector>
#include <string>
#include <array>
#include <io.h>

#include "IFBase.h"
//#include "CommonUtils.h"
#include "PipelinePathologyVirus_export.h"
namespace ALPHA
{
	namespace VIRUS
	{
		class PIPELINEPATHOLOGYVIRUS_EXPORT IFPipelinePathologyIn : public COMM::IFBase {
		public:
			explicit IFPipelinePathologyIn();
			explicit IFPipelinePathologyIn(std::string& image_folder, std::string& config_path);
			~IFPipelinePathologyIn() override {}

			std::string image_folder; // sample_name
			std::string m_image_folder;
			std::string m_slide_name;
			std::string m_wsi_preview_path;
			//libconfig::Config mConfig;

			std::string m_scan_info_path;
			std::string m_slide_preview_path;
			std::string m_config_path;
			std::array<std::string, 3> m_support_format = { ".jpg", ".tif", ".png" };
			std::vector<std::string> m_image_list;
			std::vector<std::string> m_image_prefix;
			std::string m_image_suffix;
			int m_num_image = 0;

		};
	}
}



#endif  //_IFPIPELINEPATHOLOGYIN_H