#pragma once
#if !defined(_PIPELINEELEMENTPATHOLOGYFEIAICLS_H)
#define _PIPELINEELEMENTPATHOLOGYFEIAICLS_H

#include "PipelineElementAlgorithmMachineLearningBase.h"
#include "PipelinePathologyFEIAI_export.h"
#include "IFPipelinePathologyFEIAIOut.h"
#include "IFPipelinePathologyFEIAIIn.h"
#include "InferLocalPathologyFEIAI.h"

namespace ALPHA
{
	namespace FEIAI
	{
		class PipelineElementPathologyFEIAICls : public COMM::PipelineElementAlgorithmMachineLearningBase
		{
		public:
			explicit PipelineElementPathologyFEIAICls();
			~PipelineElementPathologyFEIAICls() override;
			void setInput(std::shared_ptr< COMM::DataObj > Input) override;
			std::shared_ptr<COMM::DataObj> getOutput() override;

		private:
			std::shared_ptr<IFPipelinePathologyFEIAIIn> ptrInterfaceData;
			ALPHA::COMM::DataObj* FEIAI_out;
			


		protected:
			void preproc() override;
			void infer() override;
			void postproc() override;
		};
	}
}


#endif  //_PIPELINEELEMENTPATHOLOGYFEIAICLS_H