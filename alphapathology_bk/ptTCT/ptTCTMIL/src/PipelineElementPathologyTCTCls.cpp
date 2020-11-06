#include "PipelineElementPathologyTCTCls.h"
#include "IFPipelinePathologyTCTIn.h"


namespace ALPHA
{
	namespace TCT
	{
		PipelineElementPathologyTCTCls::PipelineElementPathologyTCTCls() :
			COMM::PipelineElementAlgorithmMachineLearningBase()
		{
			  
		}

		PipelineElementPathologyTCTCls::~PipelineElementPathologyTCTCls()
		{
			 
		}

		void PipelineElementPathologyTCTCls::setInput(std::shared_ptr< COMM::DataObj > Input)
		{
			this->ptrInterfaceData = std::dynamic_pointer_cast<IFPipelinePathologyTCTIn>(Input);
			
		}

		std::shared_ptr<COMM::DataObj> PipelineElementPathologyTCTCls::getOutput()
		{
			std::shared_ptr<COMM::DataObj> p(this->TCT_out);
			return p;
		}

		void PipelineElementPathologyTCTCls::preproc() {

		}

		void PipelineElementPathologyTCTCls::infer() {
			InferLocalPathologyTCT TCT_infer;
			//std::string model_name = this->ptrInterfaceData->mConfig.lookup("TCT_CLS_MIL.MODEL_NAME");
			TCT_infer.m_session_qc = this->m_session_qc;
			TCT_infer.m_session_cls = this->m_session_cls;
			TCT_infer.m_session_det = this->m_session_det;
			COMM::DataObj* ret = TCT_infer.predict(this->ptrInterfaceData.get(), "");
			this->TCT_out = ret;

		}

		void PipelineElementPathologyTCTCls::postproc() {

		}
	}
}