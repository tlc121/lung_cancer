#include "IFPipelinePathologyIn.h"
#include <iostream>
#include <stdio.h>

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
	namespace ROSE
	{
		IFPipelinePathologyIn::IFPipelinePathologyIn() :COMM::IFBase() {}

		IFPipelinePathologyIn::IFPipelinePathologyIn(std::string& svs_path, std::string& config_path) :COMM::IFBase()
		{
			/*
			m_image_folder_path = m_image_folder + "\\Images";
			m_slide_name = m_image_folder.substr(m_image_folder.find_last_of('\\') + 1, -1);
			m_wsi_preview_path = m_image_folder + "\\Thumbs\\Result-" + m_slide_name + ".JPG";
			m_scan_info_path = m_image_folder + "\\Scan.txt";
			m_slide_preview_path = m_image_folder + "\\Preview-" + m_slide_name + ".JPG";
			for (std::string format : m_support_format)
				get_files(m_image_folder_path, format, m_image_list);
			if (m_image_list.size() > 0)
				m_image_suffix = m_image_list[0].substr(m_image_list[0].find_last_of('.') + 1, -1);
			else
				m_image_suffix = "";
			m_num_image = m_image_list.size();
			*/
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