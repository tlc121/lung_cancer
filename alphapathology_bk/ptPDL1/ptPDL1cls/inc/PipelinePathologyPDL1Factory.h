#if !defined(_PIPELINEPATHOLOGYPDL1FACTORY_H)
#define _PIPELINEPATHOLOGYPDL1FACTORY_H

#include "PipelineFactoryBase.h"
#include "PipelinePathologyPDL1Factory_export.h"

namespace ALPHA
{
	namespace PDL1
	{
		class PipelinePathologyPDL1Factory : public COMM::PipelineFactoryBase {
		public:
			explicit PipelinePathologyPDL1Factory();
			~PipelinePathologyPDL1Factory() override;
			std::shared_ptr<COMM::PipelineBase> createPipeline() override;
			
		};
	}
}

#endif  //_PIPELINEPATHOLOGYPDL1FACTORY_H