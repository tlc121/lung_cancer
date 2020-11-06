#if !defined(_PIPELINEPATHOLOGYXSFACTORY_H)
#define _PIPELINEPATHOLOGYXSFACTORY_H

#include "PipelineFactoryBase.h"
#include "PipelinePathologyXSFactory_export.h"

namespace ALPHA
{
	namespace XS
	{
		class PipelinePathologyXSFactory : public COMM::PipelineFactoryBase {
		public:
			explicit PipelinePathologyXSFactory();
			~PipelinePathologyXSFactory() override;
			std::shared_ptr<COMM::PipelineBase> createPipeline() override;
			
		};
	}
}

#endif  //_PIPELINEPATHOLOGYXSFACTORY_H