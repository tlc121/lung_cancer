#include "IFPipelinePathologyPDL1In.h"
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
	namespace PDL1
	{
		IFPipelinePathologyPDL1In::IFPipelinePathologyPDL1In() :COMM::IFBase() {}

		IFPipelinePathologyPDL1In::IFPipelinePathologyPDL1In(std::string& image_folder, std::string& config_path) : COMM::IFBase(), m_image_folder(image_folder), m_config_path(config_path)
		{
			//LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "PathologyBaseData Object created with image_folder(from Microscope): " << image_folder << " , config_path: " << config_path);
			//COMM::loadCfg(m_config_path, mConfig);
			std::string temp = m_image_folder;
			m_config_path = config_path;
			m_slide_name = temp.substr(temp.find_last_of('\\') + 1, -1);
			m_wsi_preview_path = m_image_folder + "\\Thumbs\\Result-" + m_slide_name + ".JPG";
			m_image_folder = m_image_folder + "\\Images";
			//m_slide_name = temp.substr(temp.find_last_of('\\') + 1, -1);
			//m_wsi_preview_path = m_image_folder + "\\Thumbs\\Result-" + m_slide_name + ".JPG";
			m_scan_info_path = m_image_folder + "\\Scan.txt";
			m_slide_preview_path = m_image_folder + "\\Preview-" + m_slide_name + ".JPG";
			for (std::string format : m_support_format)
				get_files(m_image_folder, format, m_image_list);
			m_image_suffix = m_image_list[0].substr(m_image_list[0].find_last_of('.') + 1, -1);
			m_num_images = m_image_list.size();
		}
	}
}