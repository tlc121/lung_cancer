#include "IFPipelinePathologyUtilsIn.h"

namespace ALPHA
{
	namespace Pathology
	{
		namespace PTUtils
		{
			IFPipelinePathologyUtilsIn::IFPipelinePathologyUtilsIn() :COMM::IFBase() {}
			void IFPipelinePathologyUtilsIn::set_path(std::string& image, std::string& config_path) {
				this->m_img_path = image;
				this->m_config_path = config_path;
			}
		}
	}
}