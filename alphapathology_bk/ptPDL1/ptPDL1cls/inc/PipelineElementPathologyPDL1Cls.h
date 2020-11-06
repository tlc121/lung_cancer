#pragma once
#if !defined(_PIPELINEELEMENTPATHOLOGYPDL1CLS_H)
#define _PIPELINEELEMENTPATHOLOGYPDL1CLS_H

#include "PipelineElementAlgorithmMachineLearningBase.h"
#include "PipelinePathologyPDL1_export.h"
#include "IFPipelinePathologyPDL1Out.h"
#include "IFPipelinePathologyPDL1In.h"
#include "InferLocalPathologyPDL1.h"

namespace ALPHA
{
	namespace PDL1
	{
		class PipelineElementPathologyPDL1Cls : public COMM::PipelineElementAlgorithmMachineLearningBase
		{
		public:
			explicit PipelineElementPathologyPDL1Cls();
			~PipelineElementPathologyPDL1Cls() override;
			void setInput(std::shared_ptr< COMM::DataObj > Input) override;
			std::shared_ptr<COMM::DataObj> getOutput() override;
			tensorflow::Session* m_session_seg;

		private:
			std::shared_ptr<IFPipelinePathologyPDL1In> ptrInterfaceData;
			ALPHA::COMM::DataObj* PDL1_out;
			


		protected:
			void preproc() override;
			void infer() override;
			void postproc() override;
		};
	}
}


#endif  //_PIPELINEELEMENTPATHOLOGYPDL1CLS_H