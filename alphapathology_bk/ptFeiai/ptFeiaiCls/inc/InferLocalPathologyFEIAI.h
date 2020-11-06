
#if !defined(_INFERLOCALPATHOLOGYFEIAI_H)
#define _INFERLOCALPATHOLOGYFEIAI_H

#include "InferLocalBase.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <omp.h>
#include <fstream>
#include <iostream>
#include "CommonUtils.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "IFPipelinePathologyFEIAIOUT.h"
#include "IFPipelinePathologyFEIAIIn.h"
#include "AIThread.h"

namespace ALPHA
{
	namespace FEIAI
	{
		class InferLocalPathologyFEIAI : public COMM::InferLocalBase {
		public:
			explicit InferLocalPathologyFEIAI();
			
			~InferLocalPathologyFEIAI() override;
		
			COMM::DataObj* predict(COMM::DataObj* dataObj, const std::string& modelName) override;
			COMM::DataObj* getOutput();
			libconfig::Config mConfig;
		};
	}
}


#endif  //_INFERLOCALPATHOLOGYFEIAI_H