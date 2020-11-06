#include "PipelineElementPathologyUtilsCls.h"
#include "IFPipelinePathologyUtilsIn.h"



namespace ALPHA
{	

	namespace Pathology
	{ 
		namespace PTUtils
		{
			PipelineElementPathologyUtilsCls::PipelineElementPathologyUtilsCls() :
				COMM::PipelineElementAlgorithmMachineLearningBase()
			{
			}

			PipelineElementPathologyUtilsCls::~PipelineElementPathologyUtilsCls()
			{

			}

			void PipelineElementPathologyUtilsCls::setInput(std::shared_ptr< COMM::DataObj > Input)
			{
				this->ptrInterfaceData = std::dynamic_pointer_cast<IFPipelinePathologyUtilsIn>(Input);

			}

			std::shared_ptr<COMM::DataObj> PipelineElementPathologyUtilsCls::getOutput()
			{
				std::shared_ptr<COMM::DataObj> p(this->Utils_out);
				return p;
			}

			void PipelineElementPathologyUtilsCls::preproc() {

			}

			void PipelineElementPathologyUtilsCls::infer() {
				//std::string model_name = this->ptrInterfaceData->mConfig.lookup("Utils_CLS_MIL.MODEL_NAME");
				InferLocalPathologyUtils infer;
				infer.m_session = this->m_sesssion;
				COMM::DataObj* ret = infer.predict(this->ptrInterfaceData.get(), "");
				this->Utils_out = ret;

			}

			void PipelineElementPathologyUtilsCls::postproc() {

			}
		}
	}
}