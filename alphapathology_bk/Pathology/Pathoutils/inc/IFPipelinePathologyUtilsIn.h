#pragma once

#include <string>
#include "IFBase.h"
#include <vector>
#include <array>
#include <io.h>
#include "PipelinePathologyUtils_export.h"

namespace ALPHA
{
	namespace Pathology 
	{
		namespace PTUtils
		{

			class PIPELINEPATHOLOGYUTILS_EXPORT IFPipelinePathologyUtilsIn : public COMM::IFBase
			{
			public:
				explicit IFPipelinePathologyUtilsIn();


				~IFPipelinePathologyUtilsIn() override {}
				void set_path(std::string& image, std::string& config_path);
				std::string m_img_path;
				std::string m_config_path;
			};
		};
	};
};

