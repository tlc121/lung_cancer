#if !defined(_PIPELINEPATHOLOGYLungCancer_H)
#define _PIPELINEPATHOLOGYLungCancer_H

#include "PipelineAlgorithmExecutor.h"
#include "IFPipelinePathologyLungCancerOut.h"
#include "IFPipelinePathologyLungCancerIn.h"
#include "PipelinePathologyLungCancer_export.h"
class tensorflow_operator;
namespace ALPHA
{
	namespace LungCancer
	{
		class PIPELINEPATHOLOGYLUNGCANCER_EXPORT PipelinePathologyLungCancer : public COMM::PipelineAlgorithmExecutor {
		public:
			explicit PipelinePathologyLungCancer();
			explicit PipelinePathologyLungCancer(std::string config_path);
			~PipelinePathologyLungCancer() override;
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
			std::shared_ptr<IFPipelinePathologyLungCancerIn> ptrPipelineIn;
			std::shared_ptr<IFPipelinePathologyLungCancerOut> ptrPipelineOut;
			//tensorflow_operator* m_operator = NULL;
			
		};
	}
}



#endif  //_PIPELINEPATHOLOGYLungCancer_H