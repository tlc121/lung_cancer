#if !defined(_PIPELINEPATHOLOGYTCTFACTORY_H)
#define _PIPELINEPATHOLOGYTCTFACTORY_H

#include "PipelineFactoryBase.h"
#include "PipelinePathologyTCTFactory_export.h"

namespace ALPHA
{
	namespace TCT
	{
		class PipelinePathologyTCTFactory : public COMM::PipelineFactoryBase {
		public:
			explicit PipelinePathologyTCTFactory();
			~PipelinePathologyTCTFactory() override;
			std::shared_ptr<COMM::PipelineBase> createPipeline() override;
			
		};
	}
}

#endif  //_PIPELINEPATHOLOGYTCTFACTORY_H