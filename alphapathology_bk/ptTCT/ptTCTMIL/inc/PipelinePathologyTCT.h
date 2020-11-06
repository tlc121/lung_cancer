#if !defined(_PIPELINEPATHOLOGYTCT_H)
#define _PIPELINEPATHOLOGYTCT_H

#include "PipelineAlgorithmExecutor.h"
#include "IFPipelinePathologyTCTOut.h"
#include "IFPipelinePathologyTCTIn.h"
#include "PipelinePathologyTCT_export.h"
class tensorflow_operator;
namespace ALPHA
{
	namespace TCT
	{
		class PIPELINEPATHOLOGYTCT_EXPORT PipelinePathologyTCT : public COMM::PipelineAlgorithmExecutor {
		public:
			explicit PipelinePathologyTCT();
			explicit PipelinePathologyTCT(std::string config_path);
			~PipelinePathologyTCT() override;
			void setInput(std::shared_ptr<COMM::DataObj> Input) override;
			void read_qc_model();
			void read_cls_model();
			void read_det_model();
			std::shared_ptr<COMM::DataObj> getOutput() override;
			std::string m_config_path = "ccc";
		protected:
			void executePipeline() override;
			void setUpPipeline() override;
		private:
			//int i = 0;
			std::shared_ptr<IFPipelinePathologyTCTIn> ptrPipelineIn;
			std::shared_ptr<IFPipelinePathologyTCTOut> ptrPipelineOut;
			tensorflow_operator* m_operator = NULL;
			
		};
	}
}



#endif  //_PIPELINEPATHOLOGYTCT_H