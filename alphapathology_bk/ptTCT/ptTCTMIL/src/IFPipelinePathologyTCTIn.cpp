#include "IFPipelinePathologyTCTIn.h"
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
	namespace TCT
	{
		IFPipelinePathologyTCTIn::IFPipelinePathologyTCTIn() :COMM::IFBase() {}

		IFPipelinePathologyTCTIn::IFPipelinePathologyTCTIn(std::string& image_folder, std::string& config_path, bool qc, bool alg) : COMM::IFBase(), m_image_folder(image_folder), m_config_path(config_path), control(qc), algorithm(alg)
		{

			m_svs_path = m_image_folder;
			m_config_path = config_path;
			m_slide_name = m_svs_path.substr(m_svs_path.find_last_of('\\') + 1, -1);

			
			
			m_wsi_preview_path = m_image_folder + "\\Thumbs\\Result-" + m_slide_name + ".JPG";

			//m_svs_path = m_image_folder + "\\" + m_slide_name + ".svs";
			/*for (std::string format : m_support_format)
				get_files(m_image_folder, format, m_image_list);*/

			m_image_folder = m_image_folder + "\\Images";
			m_scan_info_path = m_image_folder + "\\Scan.txt";
			m_slide_preview_path = m_image_folder + "\\Preview-" + m_slide_name + ".JPG";

		}
	}
}