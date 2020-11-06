#pragma once
#if !defined(_PIPELINEELEMENTPATHOLOGYXSCLS_H)
#define _PIPELINEELEMENTPATHOLOGYXSCLS_H

#include "PipelineElementAlgorithmMachineLearningBase.h"
#include "PipelinePathologyXS_export.h"
#include "IFPipelinePathologyXSOut.h"
#include "IFPipelinePathologyXSIn.h"
#include "InferLocalPathologyXS.h"

namespace ALPHA
{
	namespace XS
	{
		class PipelineElementPathologyXSCls : public COMM::PipelineElementAlgorithmMachineLearningBase
		{
		public:
			explicit PipelineElementPathologyXSCls();
			~PipelineElementPathologyXSCls() override;
			void setInput(std::shared_ptr< COMM::DataObj > Input) override;
			std::shared_ptr<COMM::DataObj> getOutput() override;

		private:
			std::shared_ptr<IFPipelinePathologyXSIn> ptrInterfaceData;
			ALPHA::COMM::DataObj* XS_out;
			


		protected:
			void preproc() override;
			void infer() override;
			void postproc() override;
		};
	}
}


#endif  //_PIPELINEELEMENTPATHOLOGYXSCLS_H