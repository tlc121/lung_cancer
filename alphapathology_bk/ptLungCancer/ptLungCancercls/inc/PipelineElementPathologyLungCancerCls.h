#pragma once
#if !defined(_PIPELINEELEMENTPATHOLOGYLungCancerCLS_H)
#define _PIPELINEELEMENTPATHOLOGYLungCancerCLS_H

#include "PipelineElementAlgorithmMachineLearningBase.h"
#include "PipelinePathologyLungCancer_export.h"
#include "IFPipelinePathologyLungCancerOut.h"
#include "IFPipelinePathologyLungCancerIn.h"
#include "InferLocalPathologyLungCancer.h"

namespace ALPHA
{
	namespace LungCancer
	{
		class PipelineElementPathologyLungCancerCls : public COMM::PipelineElementAlgorithmMachineLearningBase
		{
		public:
			explicit PipelineElementPathologyLungCancerCls();
			~PipelineElementPathologyLungCancerCls() override;
			void setInput(std::shared_ptr< COMM::DataObj > Input) override;
			std::shared_ptr<COMM::DataObj> getOutput() override;

		private:
			std::shared_ptr<IFPipelinePathologyLungCancerIn> ptrInterfaceData;
			ALPHA::COMM::DataObj* LungCancer_out;
			


		protected:
			void preproc() override;
			void infer() override;
			void postproc() override;
		};
	}
}


#endif  //_PIPELINEELEMENTPATHOLOGYLungCancerCLS_H