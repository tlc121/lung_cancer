#ifndef _PIPELINE_FACTORY_BASE_H
#define _PIPELINE_FACTORY_BASE_H

#include <memory>
#include "PipelineBase.h"

namespace ALPHA
{
	namespace COMM
	{
		//class PipelineBase;
		class PipelineFactoryBase
		{
		public:
			explicit PipelineFactoryBase(){}
			virtual ~PipelineFactoryBase(){}
		public:
			virtual std::shared_ptr<COMM::PipelineBase> createPipeline() = 0;
		};
	}
}
#endif