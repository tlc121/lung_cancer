#pragma once
#if !defined(_PIPELINEELEMENTPATHOLOGYTCTCLS_H)
#define _PIPELINEELEMENTPATHOLOGYTCTCLS_H

#include "PipelineElementAlgorithmMachineLearningBase.h"
#include "PipelinePathologyTCT_export.h"
#include "IFPipelinePathologyTCTOut.h"
#include "IFPipelinePathologyTCTIn.h"
#include "InferLocalPathologyTCT.h"

namespace ALPHA
{
	namespace TCT
	{
		class PipelineElementPathologyTCTCls : public COMM::PipelineElementAlgorithmMachineLearningBase
		{
		public:
			explicit PipelineElementPathologyTCTCls();
			~PipelineElementPathologyTCTCls() override;
			void setInput(std::shared_ptr< COMM::DataObj > Input) override;
			std::shared_ptr<COMM::DataObj> getOutput() override;
			tensorflow::Session* m_session_qc;
			tensorflow::Session* m_session_cls;
			tensorflow::Session* m_session_det;

		private:
			std::shared_ptr<IFPipelinePathologyTCTIn> ptrInterfaceData;
			ALPHA::COMM::DataObj* TCT_out;
			


		protected:
			void preproc() override;
			void infer() override;
			void postproc() override;
		};
	}
}


#endif  //_PIPELINEELEMENTPATHOLOGYTCTCLS_H