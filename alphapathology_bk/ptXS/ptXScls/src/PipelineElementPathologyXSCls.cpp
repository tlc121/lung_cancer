#include "PipelineElementPathologyXSCls.h"
#include "IFPipelinePathologyXSIn.h"


namespace ALPHA
{
	namespace XS
	{
		PipelineElementPathologyXSCls::PipelineElementPathologyXSCls() :
			COMM::PipelineElementAlgorithmMachineLearningBase()
		{
			  
		}

		PipelineElementPathologyXSCls::~PipelineElementPathologyXSCls()
		{
			 
		}

		void PipelineElementPathologyXSCls::setInput(std::shared_ptr< COMM::DataObj > Input)
		{
			this->ptrInterfaceData = std::dynamic_pointer_cast<IFPipelinePathologyXSIn>(Input);
			
		}

		std::shared_ptr<COMM::DataObj> PipelineElementPathologyXSCls::getOutput()
		{
			std::shared_ptr<COMM::DataObj> p(this->XS_out);
			return p;
		}

		void PipelineElementPathologyXSCls::preproc() {

		}

		void PipelineElementPathologyXSCls::infer() {
			InferLocalPathologyXS XS_infer;
			//std::string model_name = this->ptrInterfaceData->mConfig.lookup("XS_CLS_MIL.MODEL_NAME");
			COMM::DataObj* ret = XS_infer.predict(this->ptrInterfaceData.get(), "");
			this->XS_out = ret;

		}

		void PipelineElementPathologyXSCls::postproc() {

		}
	}
}