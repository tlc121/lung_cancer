#if !defined(_PIPELINEPATHOLOGYXS_H)
#define _PIPELINEPATHOLOGYXS_H

#include "PipelineAlgorithmExecutor.h"
#include "IFPipelinePathologyXSOut.h"
#include "IFPipelinePathologyXSIn.h"
#include "PipelinePathologyXS_export.h"
class tensorflow_operator;
namespace ALPHA
{
	namespace XS
	{
		class PIPELINEPATHOLOGYXS_EXPORT PipelinePathologyXS : public COMM::PipelineAlgorithmExecutor {
		public:
			explicit PipelinePathologyXS();
			explicit PipelinePathologyXS(std::string config_path);
			~PipelinePathologyXS() override;
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
			std::shared_ptr<IFPipelinePathologyXSIn> ptrPipelineIn;
			std::shared_ptr<IFPipelinePathologyXSOut> ptrPipelineOut;
			//tensorflow_operator* m_operator = NULL;
			
		};
	}
}



#endif  //_PIPELINEPATHOLOGYXS_H