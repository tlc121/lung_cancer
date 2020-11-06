#include <iostream>
#include <memory>
//#include "InferLocalPathologyVirus.h"
#include "IFPipelinePathologyVirusIn.h"
#include "IFPipelinePathologyVirusOut.h"
#include "PipelinePathologyVirus.h"

int main()
{
	std::string img_folder = "D:\\fzq\\data\\J8903581#1-1";
	std::string cfg = "D:\\fzq\\data\\pathology_virus_config.cfg";
	for (int j = 0; j < 10; j++) 
	{
		//std::shared_ptr<ALPHA::VIRUS::PipelinePathologyVirusFactory> ptrFactory(new ALPHA::VIRUS::PipelinePathologyVirusFactory);
		std::shared_ptr<ALPHA::VIRUS::IFPipelinePathologyIn> ptrPipelineInput(new ALPHA::VIRUS::IFPipelinePathologyIn(img_folder, cfg));
		std::shared_ptr<ALPHA::VIRUS::PipelinePathologyVirus> pipeline(new ALPHA::VIRUS::PipelinePathologyVirus());
		//std::shared_ptr<ALPHA::COMM::PipelineBase> pipeline = ptrFactory->createPipeline();
		pipeline->setInput(ptrPipelineInput);
		pipeline->update();
		std::shared_ptr<ALPHA::COMM::DataObj> ptrOutput = pipeline->getOutput();
		std::shared_ptr<ALPHA::VIRUS::IFPipelinePathologyVirusOut> pOutput = std::dynamic_pointer_cast<ALPHA::VIRUS::IFPipelinePathologyVirusOut>(ptrOutput);

		for (int64_t i = 0; i < pOutput->preditions.size(); i++)
		{
			auto& dets = pOutput->preditions[i];
			std::cout << "Image bbox: " << pOutput->image_names[i] << std::endl;
			for (auto& d : dets) {
				std::cout << " Detection: - " << d.cls << " - " << d.conf << " - " << d.left << " - " << d.top << " - " << d.right << " - " << d.bottom << std::endl;
			}

		}
	}
	return 0;
}
