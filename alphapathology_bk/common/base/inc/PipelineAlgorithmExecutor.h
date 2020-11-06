//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Pumo
//  @ File Name : PipelineAlgorithmExecutor.h
//  @ Date : 2019/7/3
//  @ Author : weiping liu
//
//


#if !defined(_PIPELINEALGORITHMEXECUTOR_H)
#define _PIPELINEALGORITHMEXECUTOR_H

#include "PipelineAlgorithmBase.h"

namespace ALPHA
{
	namespace COMM
	{
		
		class PipelineAlgorithmExecutor : public PipelineAlgorithmBase
		{
		public:
			explicit PipelineAlgorithmExecutor():PipelineAlgorithmBase(){}
			~PipelineAlgorithmExecutor() override {}
		protected:
			void executePipeline() = 0;
			void setUpPipeline() = 0;
		};

	};
};

#endif  //_PIPELINEALGORITHMEXECUTOR_H
