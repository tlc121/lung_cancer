#include "PipelineElementPathologyLungCancerCls.h"
#include "IFPipelinePathologyLungCancerIn.h"


namespace ALPHA
{
	namespace LungCancer
	{
		PipelineElementPathologyLungCancerCls::PipelineElementPathologyLungCancerCls() :
			COMM::PipelineElementAlgorithmMachineLearningBase()
		{
			  
		}

		PipelineElementPathologyLungCancerCls::~PipelineElementPathologyLungCancerCls()
		{
			 
		}

		void PipelineElementPathologyLungCancerCls::setInput(std::shared_ptr< COMM::DataObj > Input)
		{
			this->ptrInterfaceData = std::dynamic_pointer_cast<IFPipelinePathologyLungCancerIn>(Input);
			
		}

		std::shared_ptr<COMM::DataObj> PipelineElementPathologyLungCancerCls::getOutput()
		{
			std::shared_ptr<COMM::DataObj> p(this->LungCancer_out);
			return p;
		}

		void PipelineElementPathologyLungCancerCls::preproc() {

		}

		void PipelineElementPathologyLungCancerCls::infer() {
			InferLocalPathologyLungCancer LungCancer_infer;
			//std::string model_name = this->ptrInterfaceData->mConfig.lookup("LungCancer_CLS_MIL.MODEL_NAME");
			COMM::DataObj* ret = LungCancer_infer.predict(this->ptrInterfaceData.get(), "");
			this->LungCancer_out = ret;

		}

		void PipelineElementPathologyLungCancerCls::postproc() {

		}
	}
}