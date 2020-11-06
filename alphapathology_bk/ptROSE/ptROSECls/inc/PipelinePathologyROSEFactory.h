//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : ROSE
//  @ File Name : PipelinePathologyROSEFactory.h
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//


#if !defined(_PIPELINEPATHOLOGYROSEFACTORY_H)
#define _PIPELINEPATHOLOGYROSEFACTORY_H

#include "PipelineFactoryBase.h"

namespace ALPHA
{
	namespace ROSE
	{
		class PipelinePathologyROSEFactory : public COMM::PipelineFactoryBase {
		public:
			explicit PipelinePathologyROSEFactory();
			~PipelinePathologyROSEFactory() override;
			std::shared_ptr<COMM::PipelineBase> createPipeline() override;
		};
	}
}

#endif  //_PIPELINEPATHOLOGYROSEFACTORY_H