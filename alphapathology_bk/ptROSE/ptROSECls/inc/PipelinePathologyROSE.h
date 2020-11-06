//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : ROSE
//  @ File Name : PipelinePathologyROSE.h
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//


#if !defined(_PIPELINEPATHOLOGYROSE_H)
#define _PIPELINEPATHOLOGYROSE_H

#include "PipelineAlgorithmExecutor.h"
#include "IFPipelinePathologyROSEOut.h"
#include "IFPipelinePathologyIn.h"
#include "PipelinePathologyROSE_export.h"

namespace ALPHA
{
	namespace ROSE
	{
		class PIPELINEPATHOLOGYROSE_EXPORT PipelinePathologyROSE : public ALPHA::COMM::PipelineAlgorithmExecutor {
		public:
			explicit PipelinePathologyROSE();
			~PipelinePathologyROSE() override;
			void setInput(std::shared_ptr<COMM::DataObj> Input) override;
			std::shared_ptr<COMM::DataObj> getOutput() override;
		protected:
			void executePipeline() override;
			void setUpPipeline() override;
		private:
			std::shared_ptr<ROSE::IFPipelinePathologyIn> ptrPipelineIn;
			std::shared_ptr<ROSE::IFPipelinePathologyROSEOut> ptrPipelineOut;
		};
	}
}



#endif  //_PIPELINEPATHOLOGYROSE_H