#include <iostream>
#include <memory>
#include "InferLocalPathologyLungCancer.h"
#include "IFPipelinePathologyLungCancerIn.h"
#include "IFPipelinePathologyLungCancerOut.h"
#include "PipelinePathologyLungCancer.h"
#include <time.h>






int main()
{
	std::cout << "-------test------" << std::endl;
	std::string image_folder = "D:\\data\\tif\\1730.svs";
    std::string config_path = "D:\\tlc\\huadong\\4.49\\alpha\\configuration\\pathology_config.cfg";
	bool qc = false;
	bool alg = true;
	//std::shared_ptr<ALPHA::LungCancer::PipelinePathologyLungCancerFactory> ptrFactory(new ALPHA::LungCancer::PipelinePathologyLungCancerFactory());
	std::shared_ptr<ALPHA::LungCancer::IFPipelinePathologyLungCancerIn> ptrPipelineInput(new ALPHA::LungCancer::IFPipelinePathologyLungCancerIn(image_folder, config_path));
	std::shared_ptr<ALPHA::LungCancer::PipelinePathologyLungCancer> pipeline(new ALPHA::LungCancer::PipelinePathologyLungCancer(config_path));
	//std::shared_ptr<ALPHA::COMM::PipelineBase> pipeline = ptrFactory->createPipeline();
	pipeline->setInput(ptrPipelineInput);
	
	clock_t t_update_start = clock();
	pipeline->update();
	std::cout << "alg takes: " << (clock() - t_update_start) * 1.0 / CLOCKS_PER_SEC << "s" << std::endl;

	std::shared_ptr<ALPHA::COMM::DataObj> ptroutput = pipeline->getOutput();
	std::shared_ptr<ALPHA::LungCancer::IFPipelinePathologyLungCancerOut> poutput = std::dynamic_pointer_cast<ALPHA::LungCancer::IFPipelinePathologyLungCancerOut>(ptroutput);
	std::cout << "name: " << poutput->sample_name << std::endl;
	std::cout << "result: " << poutput->sample_result << std::endl;
	std::cout << "num_bboxes: " << poutput->num_tumor_patch << std::endl;
	std::cout << "num_images:" << poutput->num_cell_patch << std::endl;

	return 0;
}