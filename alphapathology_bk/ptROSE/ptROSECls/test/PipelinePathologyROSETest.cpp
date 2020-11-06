#include <iostream>
#include <memory>
#include "InferLocalPathologyROSE.h"
#include "IFPipelinePathologyIn.h"
#include "IFPipelinePathologyROSEOut.h"
#include "PipelinePathologyROSE.h"
//#include "PipelinePathologyROSEFactory.h"
#include "EvaluationAlg.h"
#include<ctime>
clock_t start, end;

//int main()
//{
//	//for (int i = 0; i < 1; i++)
//	//{
//	start = clock();
//	/*std::cout << "-------test" << i+1 <<"------" << std::endl;*/
//	std::string image_folder = "D:\\data\\hdrose\\00920710.svs";
//	std::string config_path = "D:\\ydx\\4.40\\alpha\\configuration\\pathology_config.cfg";
//	std::shared_ptr<ALPHA::ROSE::IFPipelinePathologyIn> ptrPipelineInput(new ALPHA::ROSE::IFPipelinePathologyIn(image_folder, config_path));
//	std::shared_ptr<ALPHA::ROSE::PipelinePathologyROSE> pipeLine(new ALPHA::ROSE::PipelinePathologyROSE());
//
//	//std::cout << "-------test" << i + 1 << "------" << std::endl;
//	pipeLine->setInput(ptrPipelineInput);
//	pipeLine->update();
//	std::shared_ptr<ALPHA::COMM::DataObj> ptrOutput = pipeLine->getOutput();
//	std::shared_ptr<ALPHA::ROSE::IFPipelinePathologyROSEOut> pOutput = std::dynamic_pointer_cast<ALPHA::ROSE::IFPipelinePathologyROSEOut>(ptrOutput);
//	std::cout << "name: " << pOutput->sample_name << std::endl;
//	std::cout << "result: " << pOutput->sample_result << std::endl;
//	std::cout << "number of blur patch: " << pOutput->num_blur_patch << std::endl;
//	std::cout << "number of broken patch: " << pOutput->num_broken_patch << std::endl;
//	std::cout << "number of cell patch: " << pOutput->num_cell_patch << std::endl;
//	std::cout << "number of tumor patch: " << pOutput->num_tumor_patch << std::endl;
//	end = clock();
//	double endtime = (double)(end - start) / CLOCKS_PER_SEC;
//	std::cout << "Total time:" << endtime << " seconds" << std::endl;
//	//}
//	
//	return 0;
//}

void getFiles(std::string path, std::vector<std::string>& files) {
	intptr_t hFile = 0;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(path.append("/*").c_str(), &fileinfo)) != -1) {
		while (_findnext(hFile, &fileinfo) == 0) {
			if (strcmp(fileinfo.name, ".."))
				files.push_back(fileinfo.name);
		}
		_findclose(hFile);
	}
}


int main()
{
	std::string filePath = "E:\\zsdata\\2df9c374195bba903e72ddb5badf0a6e79ec6b99";
	std:: vector<std::string> files;
	getFiles(filePath, files);
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		std::string fname = filePath + "\\" + files[i];
		std::cout << fname << std::endl;
		start = clock();
		std::string image_folder = fname;
		std::string config_path = "E:\\tzb\\alphapathology\\configuration\\pathology_config.cfg";
		std::shared_ptr<ALPHA::ROSE::IFPipelinePathologyIn> ptrPipelineInput(new ALPHA::ROSE::IFPipelinePathologyIn(image_folder, config_path));
		std::shared_ptr<ALPHA::ROSE::PipelinePathologyROSE> pipeLine(new ALPHA::ROSE::PipelinePathologyROSE());
		pipeLine->setInput(ptrPipelineInput);
		pipeLine->update();
		std::shared_ptr<ALPHA::COMM::DataObj> ptrOutput = pipeLine->getOutput();
		std::shared_ptr<ALPHA::ROSE::IFPipelinePathologyROSEOut> pOutput = std::dynamic_pointer_cast<ALPHA::ROSE::IFPipelinePathologyROSEOut>(ptrOutput);
		std::cout << "name: " << pOutput->sample_name << std::endl;
		std::cout << "result: " << pOutput->sample_result << std::endl;
		std::cout << "number of blur patch: " << pOutput->num_blur_patch << std::endl;
		std::cout << "number of broken patch: " << pOutput->num_broken_patch << std::endl;
		std::cout << "number of cell patch: " << pOutput->num_cell_patch << std::endl;
		std::cout << "number of tumor patch: " << pOutput->num_tumor_patch << std::endl;
		end = clock();
		double endtime = (double)(end - start) / CLOCKS_PER_SEC;
		std::cout << "Total time:" << endtime << " seconds" << std::endl;
	}
}
