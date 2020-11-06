
#if !defined(_INFERLOCALPATHOLOGYUtils_H)
#define _INFERLOCALPATHOLOGYUtils_H

#include "InferLocalBase.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
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

namespace ALPHA
{
	namespace Pathology 
	{
		namespace PTUtils
		{
			class InferLocalPathologyUtils : public COMM::InferLocalBase {
			public:
				explicit InferLocalPathologyUtils();
				~InferLocalPathologyUtils() override;
				cv::Mat resize_image(cv::Mat in_img, const int input_height, const int input_width);
				cv::Mat normalize_image(cv::Mat in_img);
				std::vector<std::vector<float>> postprocess(std::vector<std::vector<float>> input, std::vector<int> ori_shape, unsigned int input_size, float score_threshold);
				std::vector<std::vector<float>> nms(std::vector<std::vector<float>> input, float iou_threshold);
				std::vector<float> iou_bboxes(std::vector<float> target_bbox, std::vector<std::vector<float>> other_bboxes);
				float iou(std::vector<float> bbox1, std::vector<float> bbox2);
				bool Mat2ChannelFirstTensor(cv::Mat in_img, tensorflow::Tensor& out_tensor, const int batch);
				COMM::DataObj* predict(COMM::DataObj* dataObj, const std::string& modelName) override;
				COMM::DataObj* getOutput();
				libconfig::Config mConfig;
				tensorflow::Session* m_session;
			};
		}
	}
}


#endif  //_INFERLOCALPATHOLOGYUtils_H