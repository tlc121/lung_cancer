
#if !defined(_INFERLOCALPATHOLOGYPDL1_H)
#define _INFERLOCALPATHOLOGYPDL1_H

#include "InferLocalBase.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <omp.h>
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
#include "IFPipelinePathologyPDL1OUT.h"
#include "IFPipelinePathologyPDL1In.h"

namespace ALPHA
{
	namespace PDL1
	{
		class InferLocalPathologyPDL1 : public COMM::InferLocalBase {
		public:
			explicit InferLocalPathologyPDL1();
			
			~InferLocalPathologyPDL1() override;
			//preprocess
			//************************************************************************************************
			cv::Mat resize_image(cv::Mat in_img, const int input_height, const int input_width);
			cv::Mat normalize_image(cv::Mat in_img);
			bool Mat2ChannelFirstTensor(cv::Mat in_img, tensorflow::Tensor& out_tensor, const int batch);
			bool Mat2ChannelLastTensor(cv::Mat in_img, tensorflow::Tensor& out_tensor, const int batch);

			//postrocess
			//************************************************************************************************
			cv::Mat vector2Mat(std::vector<std::vector<std::vector<uchar>>> input, int channel);
			std::vector<int> cal_sum(std::vector<std::vector<std::vector<uchar>>> seg_reshape);
			std::vector<std::vector<std::pair<int, int>>> FindContour(cv::Mat input);

			//************************************************************************************************
			COMM::DataObj* predict(COMM::DataObj* dataObj, const std::string& modelName) override;
			COMM::DataObj* getOutput();
			libconfig::Config mConfig;
			tensorflow::Session* m_session_seg;
		};
	}
}


#endif  //_INFERLOCALPATHOLOGYPDL1_H