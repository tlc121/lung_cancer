#if !defined(_PIPELINEPATHOLOGYUtils_H)
#define _PIPELINEPATHOLOGYUtils_H

#include "PipelineAlgorithmExecutor.h"
#include "IFPipelinePathologyUtilsOut.h"
#include "IFPipelinePathologyUtilsIn.h"
#include "PipelinePathologyUtils_export.h"

class tensorflow_operator;

namespace ALPHA
{
	namespace Pathology 
	{
		namespace PTUtils
		{
			class PIPELINEPATHOLOGYUTILS_EXPORT PipelinePathologyUtils : public COMM::PipelineAlgorithmExecutor {
			public:
				explicit PipelinePathologyUtils();
				explicit PipelinePathologyUtils(std::string config_path, std::string task_type);
				~PipelinePathologyUtils() override;
				void setInput(std::shared_ptr<COMM::DataObj> Input) override;
				std::shared_ptr<COMM::DataObj> getOutput() override;
				void read_tct_model();
				void read_rose_model();
				std::string m_config_path = "ccc";
				std::string m_task = "";
			protected:
				void executePipeline() override;
				void setUpPipeline() override;
			private:
				//int i = 0;
				std::shared_ptr<IFPipelinePathologyUtilsIn> ptrPipelineIn;
				std::shared_ptr<IFPipelinePathologyUtilsOut> ptrPipelineOut;
				tensorflow_operator* m_operator = NULL;
			};
		}
	}
}



#endif  //_PIPELINEPATHOLOGYUtils_H