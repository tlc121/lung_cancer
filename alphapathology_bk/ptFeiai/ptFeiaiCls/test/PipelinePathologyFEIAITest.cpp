#include <iostream>
#include <memory>
#include "InferLocalPathologyFEIAI.h"
#include "IFPipelinePathologyFEIAIIn.h"
#include "IFPipelinePathologyFEIAIOut.h"
#include "PipelinePathologyFEIAI.h"
#include <time.h>






int main()
{
	std::cout << "-------test------" << std::endl;
	std::string image_folder = "D:\\data\\feiai\\2020-20388-5_neg.svs";
    std::string config_path = "F:\\tlc\\alpha_pathology\\alphapathology\\configuration\\pathology_config.cfg";
	bool qc = false;
	bool alg = true;
	//std::shared_ptr<ALPHA::FEIAI::PipelinePathologyFEIAIFactory> ptrFactory(new ALPHA::FEIAI::PipelinePathologyFEIAIFactory());
	std::shared_ptr<ALPHA::FEIAI::IFPipelinePathologyFEIAIIn> ptrPipelineInput(new ALPHA::FEIAI::IFPipelinePathologyFEIAIIn(image_folder, config_path));
	std::shared_ptr<ALPHA::FEIAI::PipelinePathologyFEIAI> pipeline(new ALPHA::FEIAI::PipelinePathologyFEIAI(config_path));
	//std::shared_ptr<ALPHA::COMM::PipelineBase> pipeline = ptrFactory->createPipeline();
	pipeline->setInput(ptrPipelineInput);
	
	clock_t t_update_start = clock();
	pipeline->update();
	std::cout << "alg takes: " << (clock() - t_update_start) * 1.0 / CLOCKS_PER_SEC << "s" << std::endl;

	std::shared_ptr<ALPHA::COMM::DataObj> ptroutput = pipeline->getOutput();
	std::shared_ptr<ALPHA::FEIAI::IFPipelinePathologyFEIAIOut> poutput = std::dynamic_pointer_cast<ALPHA::FEIAI::IFPipelinePathologyFEIAIOut>(ptroutput);
	std::cout << "name: " << poutput->sample_name << std::endl;
	std::cout << "result: " << poutput->sample_result << std::endl;
	std::cout << "num_bboxes: " << poutput->num_tumor_patch << std::endl;
	std::cout << "num_images:" << poutput->num_cell_patch << std::endl;

	return 0;
}