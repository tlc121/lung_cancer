#if !defined(_PIPELINEPATHOLOGYFEIAI_H)
#define _PIPELINEPATHOLOGYFEIAI_H

#include "PipelineAlgorithmExecutor.h"
#include "IFPipelinePathologyFEIAIOut.h"
#include "IFPipelinePathologyFEIAIIn.h"
#include "PipelinePathologyFEIAI_export.h"
class tensorflow_operator;
namespace ALPHA
{
	namespace FEIAI
	{
		class PIPELINEPATHOLOGYFEIAI_EXPORT PipelinePathologyFEIAI : public COMM::PipelineAlgorithmExecutor {
		public:
			explicit PipelinePathologyFEIAI();
			explicit PipelinePathologyFEIAI(std::string config_path);
			~PipelinePathologyFEIAI() override;
			void setInput(std::shared_ptr<COMM::DataObj> Input) override;
			/*void read_qc_model();
			void read_cls_model();
			void read_det_model();*/
			std::shared_ptr<COMM::DataObj> getOutput() override;
			std::string m_config_path = "ccc";
		protected:
			void executePipeline() override;
			void setUpPipeline() override;
		private:
			//int i = 0;
			std::shared_ptr<IFPipelinePathologyFEIAIIn> ptrPipelineIn;
			std::shared_ptr<IFPipelinePathologyFEIAIOut> ptrPipelineOut;
			//tensorflow_operator* m_operator = NULL;
			
		};
	}
}



#endif  //_PIPELINEPATHOLOGYFEIAI_H