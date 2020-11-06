#if !defined(_PIPELINEPATHOLOGYPDL1_H)
#define _PIPELINEPATHOLOGYPDL1_H

#include "PipelineAlgorithmExecutor.h"
#include "IFPipelinePathologyPDL1Out.h"
#include "IFPipelinePathologyPDL1In.h"
#include "PipelinePathologyPDL1_export.h"
class tensorflow_operator;
namespace ALPHA
{
	namespace PDL1
	{
		class PIPELINEPATHOLOGYPDL1_EXPORT PipelinePathologyPDL1 : public COMM::PipelineAlgorithmExecutor {
		public:
			explicit PipelinePathologyPDL1();
			explicit PipelinePathologyPDL1(std::string config_path);
			~PipelinePathologyPDL1() override;
			void setInput(std::shared_ptr<COMM::DataObj> Input) override;
			void read_seg_model();
			std::shared_ptr<COMM::DataObj> getOutput() override;
			std::string m_config_path = "ccc";
		protected:
			void executePipeline() override;
			void setUpPipeline() override;
		private:
			//int i = 0;
			std::shared_ptr<IFPipelinePathologyPDL1In> ptrPipelineIn;
			std::shared_ptr<IFPipelinePathologyPDL1Out> ptrPipelineOut;
			tensorflow_operator* m_operator = NULL;
			
		};
	}
}



#endif  //_PIPELINEPATHOLOGYPDL1_H