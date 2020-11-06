#include "PipelineElementPathologyPDL1Cls.h"
#include "IFPipelinePathologyPDL1In.h"


namespace ALPHA
{
	namespace PDL1
	{
		PipelineElementPathologyPDL1Cls::PipelineElementPathologyPDL1Cls() :
			COMM::PipelineElementAlgorithmMachineLearningBase()
		{
			  
		}

		PipelineElementPathologyPDL1Cls::~PipelineElementPathologyPDL1Cls()
		{
			 
		}

		void PipelineElementPathologyPDL1Cls::setInput(std::shared_ptr< COMM::DataObj > Input)
		{
			this->ptrInterfaceData = std::dynamic_pointer_cast<IFPipelinePathologyPDL1In>(Input);
			
		}

		std::shared_ptr<COMM::DataObj> PipelineElementPathologyPDL1Cls::getOutput()
		{
			std::shared_ptr<COMM::DataObj> p(this->PDL1_out);
			return p;
		}

		void PipelineElementPathologyPDL1Cls::preproc() {

		}

		void PipelineElementPathologyPDL1Cls::infer() {
			InferLocalPathologyPDL1 PDL1_infer;
			//std::string model_name = this->ptrInterfaceData->mConfig.lookup("PDL1_CLS_MIL.MODEL_NAME");
			PDL1_infer.m_session_seg = this->m_session_seg;
			COMM::DataObj* ret = PDL1_infer.predict(this->ptrInterfaceData.get(), "");
			this->PDL1_out = ret;

		}

		void PipelineElementPathologyPDL1Cls::postproc() {

		}
	}
}