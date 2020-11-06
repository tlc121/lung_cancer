#include "IFPipelinePathologyFEIAIIn.h"
#include <iostream>

int get_files(std::string fileFolderPath, std::string fileExtension, std::vector<std::string>& file)
{
	std::string fileFolder = fileFolderPath + "\\*" + fileExtension;
	std::string fileName;
	struct _finddata_t fileInfo;
	long long findResult = _findfirst(fileFolder.c_str(), &fileInfo);
	if (findResult == -1)
	{
		_findclose(findResult);
		return 0;
	}
	bool flag = 0;

	do
	{
		fileName = fileFolderPath + "\\" + fileInfo.name;
		if (fileInfo.attrib == _A_ARCH)
		{
			file.push_back(fileName);
		}
	} while (_findnext(findResult, &fileInfo) == 0);

	_findclose(findResult);
}

namespace ALPHA
{
	namespace FEIAI
	{
		IFPipelinePathologyFEIAIIn::IFPipelinePathologyFEIAIIn() :COMM::IFBase() {}

		IFPipelinePathologyFEIAIIn::IFPipelinePathologyFEIAIIn(std::string& svs_path, std::string& config_path) : COMM::IFBase()
		{
			m_svs_path = svs_path;
			m_config_path = config_path;

			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
			_splitpath(svs_path.c_str(), drive, dir, fname, ext);
			for (std::string format : m_svs_support_format)
			{
				if (format == ext)
				{
					m_format_dismatch = false;
				}
			}
			m_slide_name = fname;
		}
	}
}