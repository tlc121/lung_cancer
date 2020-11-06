#include <iostream>
#include <memory>
#include "InferLocalPathologyXS.h"
#include "IFPipelinePathologyXSIn.h"
#include "IFPipelinePathologyXSOut.h"
#include "PipelinePathologyXS.h"
#include <time.h>






int main()
{
	std::cout << "-------test------" << std::endl;
	std::string image_folder = "D:\\data\\tif\\XS\\821cbf2e0f5bd796add0743cde328379b5e74a3f.svs";
    std::string config_path = "D:\\tlc\\huadong\\4.49\\alpha\\configuration\\pathology_config.cfg";
	bool qc = false;
	bool alg = true;
	//std::shared_ptr<ALPHA::XS::PipelinePathologyXSFactory> ptrFactory(new ALPHA::XS::PipelinePathologyXSFactory());
	std::shared_ptr<ALPHA::XS::IFPipelinePathologyXSIn> ptrPipelineInput(new ALPHA::XS::IFPipelinePathologyXSIn(image_folder, config_path));
	std::shared_ptr<ALPHA::XS::PipelinePathologyXS> pipeline(new ALPHA::XS::PipelinePathologyXS(config_path));
	//std::shared_ptr<ALPHA::COMM::PipelineBase> pipeline = ptrFactory->createPipeline();
	pipeline->setInput(ptrPipelineInput);
	
	clock_t t_update_start = clock();
	pipeline->update();
	std::cout << "alg takes: " << (clock() - t_update_start) * 1.0 / CLOCKS_PER_SEC << "s" << std::endl;

	std::shared_ptr<ALPHA::COMM::DataObj> ptroutput = pipeline->getOutput();
	std::shared_ptr<ALPHA::XS::IFPipelinePathologyXSOut> poutput = std::dynamic_pointer_cast<ALPHA::XS::IFPipelinePathologyXSOut>(ptroutput);
	std::cout << "name: " << poutput->sample_name << std::endl;
	std::cout << "result: " << poutput->sample_result << std::endl;
	std::cout << "num_bboxes: " << poutput->num_tumor_patch << std::endl;

	return 0;
}