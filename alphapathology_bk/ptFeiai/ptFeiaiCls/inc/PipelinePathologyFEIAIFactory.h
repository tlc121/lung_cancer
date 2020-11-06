#if !defined(_PIPELINEPATHOLOGYFEIAIFACTORY_H)
#define _PIPELINEPATHOLOGYFEIAIFACTORY_H

#include "PipelineFactoryBase.h"
#include "PipelinePathologyFEIAIFactory_export.h"

namespace ALPHA
{
	namespace FEIAI
	{
		class PipelinePathologyFEIAIFactory : public COMM::PipelineFactoryBase {
		public:
			explicit PipelinePathologyFEIAIFactory();
			~PipelinePathologyFEIAIFactory() override;
			std::shared_ptr<COMM::PipelineBase> createPipeline() override;
			
		};
	}
}

#endif  //_PIPELINEPATHOLOGYFEIAIFACTORY_H