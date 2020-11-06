#include "PipelineElementPathologyFEIAICls.h"
#include "IFPipelinePathologyFEIAIIn.h"


namespace ALPHA
{
	namespace FEIAI
	{
		PipelineElementPathologyFEIAICls::PipelineElementPathologyFEIAICls() :
			COMM::PipelineElementAlgorithmMachineLearningBase()
		{
			  
		}

		PipelineElementPathologyFEIAICls::~PipelineElementPathologyFEIAICls()
		{
			 
		}

		void PipelineElementPathologyFEIAICls::setInput(std::shared_ptr< COMM::DataObj > Input)
		{
			this->ptrInterfaceData = std::dynamic_pointer_cast<IFPipelinePathologyFEIAIIn>(Input);
			
		}

		std::shared_ptr<COMM::DataObj> PipelineElementPathologyFEIAICls::getOutput()
		{
			std::shared_ptr<COMM::DataObj> p(this->FEIAI_out);
			return p;
		}

		void PipelineElementPathologyFEIAICls::preproc() {

		}

		void PipelineElementPathologyFEIAICls::infer() {
			InferLocalPathologyFEIAI FEIAI_infer;
			//std::string model_name = this->ptrInterfaceData->mConfig.lookup("FEIAI_CLS_MIL.MODEL_NAME");
			COMM::DataObj* ret = FEIAI_infer.predict(this->ptrInterfaceData.get(), "");
			this->FEIAI_out = ret;

		}

		void PipelineElementPathologyFEIAICls::postproc() {

		}
	}
}