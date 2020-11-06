#ifndef _AI_THREAD_H_
#define _AI_THREAD_H_


#include <thread>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <omp.h>
#include <fstream>
#include <iostream>
#include <algorithm>
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
#include "IFPipelinePathologyTCTOUT.h"
#include "IFPipelinePathologyTCTIn.h"
#include <stdint.h>
#include "openslide-features.h"
#include "openslide.h"
#include "IoThread.h"

namespace ALPHA
{
	namespace TCT {

		class AIThread
		{
		private:
			void run();

			/*preprocess
			*************************************************************************************************/
			cv::Mat resize_image(cv::Mat in_img, const int input_height, const int input_width);
			cv::Mat normalize_image(cv::Mat in_img);
			float cal_var(std::vector<int> im_flat);
			bool Mat2ChannelFirstTensor(cv::Mat in_img, tensorflow::Tensor& out_tensor, const int batch); //将channel first的数据放入tensor (对应pytorch训练的模型)
			bool Mat2ChannelLastTensor(cv::Mat in_img, tensorflow::Tensor& out_tensor, const int batch);  //将channel last的数据放入tensor (对应tensorflow训练的模型)
			cv::Mat cam_resize(cv::Mat input, const int input_height, const int input_width);


			//quality control & classification
			//************************************************************************************************
			void Quality_control(IFPipelinePathologyTCTOut* output, IFPipelinePathologyTCTIn* input, tensorflow::Tensor input_tensor, tensorflow::Session* session_qc);
			std::pair<std::vector<float>, float> Algorithm(IFPipelinePathologyTCTOut* output, IFPipelinePathologyTCTIn* input, tensorflow::Tensor input_tensor, tensorflow::Session* session_cls);
			std::vector<std::vector<std::vector<float>>> cam_calculation(std::vector<std::vector<std::vector<std::vector<float>>>> cam_fm_reshape, std::vector<float> weights);
			cv::Mat vector2Mat(std::vector<std::vector<float>> input);
			float summ_cam(cv::Mat cam, std::vector<int> bbox);

			//dentection alg
			//************************************************************************************************
			std::vector<std::vector<float>> postprocess(std::vector<std::vector<float>> input, std::vector<int> ori_shape, unsigned int input_size, float score_threshold);
			std::vector<std::vector<float>> nms(std::vector<std::vector<float>> input, float iou_threshold);
			std::vector<float> iou_bboxes(std::vector<float> target_bbox, std::vector<std::vector<float>> other_bboxes);
			float iou(std::vector<float> bbox1, std::vector<float> bbox2);
			std::vector<std::vector<std::pair<std::vector<int>, float>>> Detection_batch(std::vector<float> cls_prob, std::vector<std::pair<int, int>> wsi_pos, std::pair<int, int> img_shape, tensorflow::Tensor input_tensor, tensorflow::Session* session_det, std::vector<std::vector<std::vector<float>>> cam_batch);


			std::list<std::vector<std::vector<float>>> cam_ret;
			IFPipelinePathologyTCTOut* m_output_TCT;
			IFPipelinePathologyTCTIn* m_input_TCT;
			tensorflow::Session* session_qc;
			tensorflow::Session* session_cls;
			tensorflow::Session* session_det;

			//parameters we need in alg
			int m_width_cls = 0;
			int m_height_cls = 0;
			int m_width_qc = 0;
			int m_height_qc = 0;
			int m_cut_width = 0;
			int m_cut_height = 0;
			float m_prob_threshold_cls;
			float m_prob_threshold_det;
			float m_prob_threshold_det_AI;
			float m_prob_threshold_cls_AI;
			float m_cell_prob_threshold;
			int m_batch_size_cls;
			int m_batch_size_det;
			int m_num_threshold;
			std::string type0;
			std::string type1;
			bool m_algorithm;
			bool m_quality_control;

			std::string m_input_node_det;
			std::string m_output_node_sbbox; 
			std::string m_output_node_mbbox;
			std::string m_output_node_lbbox;
			
			float m_iou_threshold;
			float m_cam_threshold;
			int m_num_classes;

			std::string m_qc_input_node; 
			std::string m_qc_output_node;
			float m_prob_blur_threshold;
			float m_prob_zazhi_threshold;

			std::string m_input_node_cls;
			std::string m_output_node_cls;
			std::string m_output_cam;
			std::string m_weights_path;


			std::vector<std::pair<int, int>> init_pos;
			std::vector<std::pair<cv::Mat, std::pair<int, int>>> cls_img;
			std::list<std::pair<cv::Mat, std::pair<int, int>>> detection_img; //store img and its initial position
			std::list<float> cls_probs; //store img and its initial position
			std::list<std::pair<int, int>> start_pos;
			std::vector<std::pair<std::vector<int>, float>> final_bbox_res;
			std::vector<std::pair<std::vector<int>, float>> lsil_bboxes;
			std::vector<std::pair<std::vector<int>, float>> hsil_bboxes;
			std::vector<std::pair<std::vector<int>, float>> lsil_ret;
			std::vector<std::pair<std::vector<int>, float>> hsil_ret;
			int patch_index = 0;
			int batch_index_cls = 0;
			int image_idx = 0;
			int image_with_cells = 0;
			ALPHA::TCT::IoThread* m_io;

		public:
			explicit AIThread() {}
			explicit AIThread(IFPipelinePathologyTCTOut* output, IFPipelinePathologyTCTIn* input, tensorflow::Session* session_qc, tensorflow::Session* session_cls, tensorflow::Session* session_det, ALPHA::TCT::IoThread* io);
			~AIThread() {
			}
			void startThread();
		};
	}
}
#endif