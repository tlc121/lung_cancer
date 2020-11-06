#pragma once
#if !defined(_PIPELINEELEMENTPATHOLOGYUtilsCLS_H)
#define _PIPELINEELEMENTPATHOLOGYUtilsCLS_H

#include "PipelineElementAlgorithmMachineLearningBase.h"
#include "PipelinePathologyUtils_export.h"
#include "IFPipelinePathologyUtilsOut.h"
#include "IFPipelinePathologyUtilsIn.h"
#include "InferLocalPathologyUtils.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"

namespace ALPHA
{
	namespace Pathology 
	{
		namespace PTUtils
		{
			class PipelineElementPathologyUtilsCls : public COMM::PipelineElementAlgorithmMachineLearningBase
			{
			public:
				explicit PipelineElementPathologyUtilsCls();
				~PipelineElementPathologyUtilsCls() override;
				void setInput(std::shared_ptr< COMM::DataObj > Input) override;
				std::shared_ptr<COMM::DataObj> getOutput() override;
				tensorflow::Session* m_sesssion;

			private:
				std::shared_ptr<IFPipelinePathologyUtilsIn> ptrInterfaceData;
				ALPHA::COMM::DataObj* Utils_out;

			protected:
				void preproc() override;
				void infer() override;
				void postproc() override;
			};
		}
	}
}


#endif  //_PIPELINEELEMENTPATHOLOGYUtilsCLS_H