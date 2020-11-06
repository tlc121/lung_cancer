//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Pumo
//  @ File Name : PipelineAlgorithmBase.h
//  @ Date : 2019/7/3
//  @ Author : weiping liu
//
//


#if !defined(_PIPELINEALGORITHMBASE_H)
#define _PIPELINEALGORITHMBASE_H

#include <list>
#include <memory>
#include "PipelineBase.h"
#include "PipelineElementBase.h"

namespace ALPHA
{
	namespace COMM
	{
				
		class PipelineAlgorithmBase : virtual public PipelineBase
		{
		public:
			explicit PipelineAlgorithmBase():PipelineBase(),listElement(){}
			~PipelineAlgorithmBase() override {}
			virtual void update()
			{
				listElement.clear();
				setUpPipeline();
				executePipeline();
			}
		protected:
			std::list<std::shared_ptr<PipelineElementBase>> listElement;
			virtual void setUpPipeline() = 0;
			virtual void executePipeline() = 0;

		};

	};
};

#endif  //_PIPELINEALGORITHMBASE_H