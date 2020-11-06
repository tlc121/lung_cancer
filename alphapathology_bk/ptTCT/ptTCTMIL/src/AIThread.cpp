#include "AIThread.h"


namespace ALPHA
{
	namespace TCT {

		bool cmp(const std::pair<std::vector<int>, float> a, const std::pair<std::vector<int>, float> b) {
			return a.second > b.second;//自定义的比较函数用来做检测框概率分类
		}

		AIThread::AIThread(IFPipelinePathologyTCTOut* output, IFPipelinePathologyTCTIn* input, tensorflow::Session* session_qc, tensorflow::Session* session_cls, tensorflow::Session* session_det, ALPHA::TCT::IoThread* io) {
			m_input_TCT = input;
			m_output_TCT = output;
			this->session_qc = session_qc;
			this->session_cls = session_cls;
			this->session_det = session_det;
			m_io = io;
			
			libconfig::Config mConfig;
			COMM::loadCfg(m_input_TCT->m_config_path, mConfig);

			libconfig::Setting& root = mConfig.getRoot();
			libconfig::Setting& shape_cls = root["TCT_PREPROCESS"]["CLS_INPUT_SHAPE"];
			libconfig::Setting& shape_qc = root["TCT_PREPROCESS"]["QC_INPUT_SHAPE"];

			m_width_cls = shape_cls[0];
			m_height_cls = shape_cls[1];
			m_width_qc = shape_qc[0];
			m_height_qc = shape_qc[1];

			m_cut_width = 645;
			m_cut_height = 608;

			m_prob_threshold_cls = mConfig.lookup("TCT_CLS_MIL.PROB_CLS_THRESHOLD");
			m_prob_threshold_det = mConfig.lookup("TCT_CLS_MIL.PROB_DET_THRESHOLD");
			m_prob_threshold_det_AI = mConfig.lookup("TCT_CLS_MIL.PROB_DET_THRESHOLD_AI");
			m_prob_threshold_cls_AI = mConfig.lookup("TCT_CLS_MIL.PROB_CLS_THRESHOLD_AI");
			m_cell_prob_threshold = mConfig.lookup("TCT_CLS_MIL.CELL_THRESHOLD_AI");
			m_batch_size_cls = mConfig.lookup("TCT_CLS_MIL.BATCH_SIZE_CLS");
			m_batch_size_det = mConfig.lookup("TCT_CLS_MIL.BATCH_SIZE_DET");
			m_num_threshold = mConfig.lookup("TCT_CLS_MIL.NUM_PATCH_THRESHOLD");

			m_qc_input_node = mConfig.lookup("TCT_CLS_MIL.INPUT_QC_NODE").c_str();
			m_qc_output_node = mConfig.lookup("TCT_CLS_MIL.OUTPUT_QC_NODE").c_str();
			m_prob_blur_threshold = mConfig.lookup("TCT_CLS_MIL.PROB_BLUR_THRESHOLD");
			m_prob_zazhi_threshold = mConfig.lookup("TCT_CLS_MIL.PROB_ZAZHI_THRESHOLD");

			m_input_node_cls = mConfig.lookup("TCT_CLS_MIL.INPUT_CLS_NODE").c_str();
			m_output_node_cls = mConfig.lookup("TCT_CLS_MIL.OUTPUT_CLS_NODE_1").c_str();
			m_output_cam = mConfig.lookup("TCT_CLS_MIL.OUTPUT_CLS_NODE_2").c_str();

			m_input_node_det = mConfig.lookup("TCT_CLS_MIL.INPUT_DET_NODE").c_str();
			m_output_node_sbbox = mConfig.lookup("TCT_CLS_MIL.OUTPUT_DET_NODE_1").c_str();
			m_output_node_mbbox = mConfig.lookup("TCT_CLS_MIL.OUTPUT_DET_NODE_2").c_str();
			m_output_node_lbbox = mConfig.lookup("TCT_CLS_MIL.OUTPUT_DET_NODE_3").c_str();
			m_iou_threshold = mConfig.lookup("TCT_CLS_MIL.IOU_DET_THRESHOLD");
			m_cam_threshold = mConfig.lookup("TCT_CLS_MIL.CAM_THRESHOLD");
			m_num_classes = mConfig.lookup("TCT_CLS_MIL.NUM_CLASSES_DET");
			this->type0 = type0;
			this->type1 = type1;
			m_algorithm = m_input_TCT->algorithm;
			m_quality_control = m_input_TCT->control;

		}

		//**********************************************************************************************************************
		cv::Mat AIThread::resize_image(cv::Mat in_img, const int input_width, const int input_height)
		{
			cv::Size dsize = cv::Size(in_img.cols, in_img.rows);
			cv::Mat out_img = cv::Mat(dsize, CV_8UC3);
			cv::resize(in_img, out_img, cv::Size(input_width, input_height));
			return out_img;
		}
		//**********************************************************************************************************************
		cv::Mat AIThread::cam_resize(cv::Mat in_img, const int input_width, const int input_height) {
			cv::Size dsize = cv::Size(in_img.cols, in_img.rows);
			cv::Mat out_img = cv::Mat(dsize, CV_32FC1);
			cv::resize(in_img, out_img, cv::Size(input_width, input_height));
			return out_img;
		}

		//**********************************************************************************************************************
		float AIThread::cal_var(std::vector<int> im_flat)
		{
			float sum = 0;
			float mean = 0;
			int len = im_flat.size();
			for (int i = 0; i < len; i++)
				sum += im_flat[i];
			mean = sum / len;

			sum = 0;
			for (int i = 0; i < len; i++)
				sum += pow(im_flat[i] - mean, 2);
			return sum / len;
		}
		//**********************************************************************************************************************
		bool AIThread::Mat2ChannelFirstTensor(cv::Mat in_img, tensorflow::Tensor& out_tensor, const int batch)
		{
			auto out_tensorMapped = out_tensor.tensor<float, 4>();
			int height = in_img.rows;
			int width = in_img.cols;
			int depth = in_img.channels();
			const float* data = (float*)in_img.data;
			for (int y = 0; y < height; y++)
			{
				const float* dataRow = data + (y * width * depth);
				for (int x = 0; x < width; x++)
				{
					const float* dataPixel = dataRow + (x * depth);
					for (int c = 0; c < depth; c++)
					{
						const float* dataValue = dataPixel + c;
						out_tensorMapped(batch, 2 - c, y, x) = *dataValue;
					}
				}
			}
			return true;
		}
		//**********************************************************************************************************************
		cv::Mat AIThread::normalize_image(cv::Mat in_img)
		{
			int rows = in_img.rows;
			int cols = in_img.cols;

			//create a float Mat to store the pixel intensity
			cv::Mat norm_img = cv::Mat(rows, cols, CV_32FC3, cv::Scalar::all(0));
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					cv::Vec3b pixel = in_img.at<cv::Vec3b>(i, j);
					uchar red = pixel.val[0];
					uchar green = pixel.val[1];
					uchar blue = pixel.val[2];

					float replace_b = blue / 255.0;
					float replace_g = green / 255.0;
					float replace_r = red / 255.0;

					float* r = &norm_img.at<cv::Vec3f>(i, j)[0];
					float* g = &norm_img.at<cv::Vec3f>(i, j)[1];
					float* b = &norm_img.at<cv::Vec3f>(i, j)[2];

					*b = replace_b;
					*g = replace_g;
					*r = replace_r;

				}
			}
			return norm_img;
		}
		//**********************************************************************************************************************

		float AIThread::summ_cam(cv::Mat cam, std::vector<int> bbox) {
			int xmin = bbox[0];
			int ymin = bbox[1];
			int xmax = bbox[2];
			int ymax = bbox[3];

			float summ = 0;
			for (int i = ymin; i < ymax; i++) {
				for (int j = xmin; j < xmax; j++) {
					float pixel = cam.at<float>(i, j);

					summ += pixel;
				}
			}
			return summ;
		}
		//**********************************************************************************************************************
		bool AIThread::Mat2ChannelLastTensor(cv::Mat in_img, tensorflow::Tensor& out_tensor, const int batch)
		{
			auto out_tensorMapped = out_tensor.tensor<float, 4>();
			int height = in_img.rows;
			int width = in_img.cols;
			int depth = in_img.channels();
			const float* data = (float*)in_img.data;

			for (int y = 0; y < height; y++)
			{
				const float* dataRow = data + (y * width * depth);
				for (int x = 0; x < width; x++)
				{
					const float* dataPixel = dataRow + (x * depth);
					for (int c = 0; c < depth; c++)
					{
						const float* dataValue = dataPixel + c;
						out_tensorMapped(batch, y, x, c) = *dataValue;
					}
				}
			}
			return true;
		}
		//**********************************************************************************************************************
		cv::Mat AIThread::vector2Mat(std::vector<std::vector<float>> input) {
			cv::Mat ret(input.size(), input[0].size(), CV_32FC1, cv::Scalar::all(0));
			for (int i = 0; i < input.size(); i++) {
				float* data_mat = ret.ptr<float>(i);
				for (int j = 0; j < input[0].size(); j++) {
					data_mat[j] = input[i][j];
				}
			}
			cv::Mat resize_img = this->cam_resize(ret, input.size() * 32, input.size() * 32);
			return resize_img;

		}
		//**********************************************************************************************************************
		void AIThread::Quality_control(IFPipelinePathologyTCTOut* output, IFPipelinePathologyTCTIn* input, tensorflow::Tensor input_tensor, tensorflow::Session* session_qc) {

			std::vector<tensorflow::Tensor> qc_outputs;
			std::string qc_input = m_qc_input_node;
			std::string qc_output = m_qc_output_node;
			std::pair<std::string, tensorflow::Tensor>img_qc_feed(qc_input, input_tensor);
			tensorflow::Status status_qc = session_qc->Run({ img_qc_feed }, { qc_output }, {}, &qc_outputs);

			tensorflow::Tensor qc_t = qc_outputs[0];

			auto qc_probilities = qc_t.matrix<float_t>();

			//take 6 probilities out from tensor
			for (unsigned int i = 0; i < m_batch_size_cls; i++) {
				float blur_probs = qc_probilities(i, 1);
				float zazhi_probs = qc_probilities(i, 2);
				if (blur_probs > m_prob_blur_threshold) {
					output->blur_images += 1;
				}

				if (zazhi_probs > m_prob_zazhi_threshold) {
					output->zazhi_images += 1;
				}
			}
			return;
		}
		//**********************************************************************************************************************
		std::vector<std::vector<std::vector<float>>> AIThread::cam_calculation(std::vector<std::vector<std::vector<std::vector<float>>>> cam_fm_reshape, std::vector<float> weights) {
			int batch_size = cam_fm_reshape.size();
			int num_features = cam_fm_reshape[0].size();
			int h = cam_fm_reshape[0][0].size();
			int w = cam_fm_reshape[0][0][0].size();

			std::vector<std::vector<std::vector<float>>> cam_ret(batch_size, std::vector<std::vector<float>>(h, std::vector<float>(w, 0.0)));


			//multiply with weights
			for (unsigned int i = 0; i < batch_size; i++) {
				for (unsigned int j = 0; j < num_features; j++) {
					for (unsigned int k = 0; k < h; k++) {
						for (unsigned int l = 0; l < w; l++) {
							float temp_fm = cam_fm_reshape[i][j][k][l] * weights[j];
							cam_fm_reshape[i][j][k][l] = temp_fm;
						}
					}
				}
			}

			//get average
			for (unsigned int i = 0; i < batch_size; i++) {
				for (unsigned int j = 0; j < h; j++) {
					for (unsigned int k = 0; k < w; k++) {
						float summ = 0;
						for (unsigned int l = 0; l < num_features; l++) {
							summ += cam_fm_reshape[i][l][j][k];
						}
						if (summ < 0)
							summ = 0;
						cam_ret[i][j][k] = summ;
					}
				}
			}

			return cam_ret;
		}
		//**********************************************************************************************************************
		std::pair<std::vector<float>, float> AIThread::Algorithm(IFPipelinePathologyTCTOut* output, IFPipelinePathologyTCTIn* input, tensorflow::Tensor input_tensor, tensorflow::Session* session_cls) {
			
			//do the inference with a batchsize equals to 6
			std::string alg_input = m_input_node_cls;
			std::vector<tensorflow::Tensor> outputs_0;
			std::vector<tensorflow::Tensor> outputs_1;

			//cam
			std::string prob_output = m_output_node_cls;
			std::string cam_output = m_output_cam;

			std::pair<std::string, tensorflow::Tensor>img_feed(alg_input, input_tensor);
			tensorflow::Status status = session_cls->Run({ img_feed }, { prob_output }, {}, &outputs_0);
			tensorflow::Status status1 = session_cls->Run({ img_feed }, { cam_output }, {}, &outputs_1);

			std::pair<std::vector<float>, float> ret;
			float max_prob = 0.0;
			std::vector<float> probs;

			int batchsize = outputs_1[0].dim_size(0);
			int h_size = outputs_1[0].dim_size(3);
			int w_size = outputs_1[0].dim_size(2);


			tensorflow::Tensor t = outputs_0[0];
			tensorflow::Tensor cam = outputs_1[0];

			auto probilities = t.matrix<float_t>();
			auto cam_fm = cam.flat<float>();


			std::vector<std::vector<std::vector<float>>> cam_fm_reshape(batchsize, std::vector<std::vector<float>>(h_size, std::vector<float>(w_size, 0.0)));
			int idx_count = 0;
			for (unsigned int i = 0; i < batchsize; i++) {
				for (unsigned int k = 0; k < h_size; k++) {
					for (unsigned int l = 0; l < w_size; l++) {
						float temp_fm = cam_fm(idx_count);
						cam_fm_reshape[i][k][l] = temp_fm;
						idx_count++;
					}
				}
			}

			output->cam_ret = cam_fm_reshape;
			//take 6 probilities out from tensor
			for (unsigned int i = 0; i < m_batch_size_cls; i++) {
				float prob_temp = probilities(i, 1);
				if (prob_temp > max_prob) {
					max_prob = prob_temp;
				}
				probs.push_back(prob_temp);
			}

			ret.first = probs;
			ret.second = max_prob;
			return ret;
		}

		//***************************************************************************************************************************************
		std::vector<std::vector<float>> AIThread::postprocess(std::vector<std::vector<float>> input, std::vector<int> ori_shape, unsigned int input_size, float score_threshold) {
			int row = input.size();
			int col = input[0].size();

			int ori_h = ori_shape[0];
			int ori_w = ori_shape[1];

			int final_dimension = 0;
			std::vector<std::vector<float>> output;
			std::vector<std::vector<float>> pred_xywh(row, std::vector<float>(4, 0.0));//bbox
			std::vector<std::vector<float>> pred_conf(row, std::vector<float>(1, 0.0));//confidence
			std::vector<std::vector<float>> pred_prob(row, std::vector<float>(col - 5, 0.0));//classification 0 or 1
			std::vector<std::vector<bool>> scale_mask(row, std::vector<bool>(1, true));//matrix which stores some invalid box scale
			std::vector<std::vector<int>> classes(row, std::vector<int>(1, 0)); //which class has the highest probility
			std::vector<std::vector<bool>> score_mask(row, std::vector<bool>(1, false)); //box whose score is higher than threshold
			std::vector<std::vector<bool>> mask(row, std::vector<bool>(1, false)); //final decision of bboxes

			//give pred_xywh, pred_conf, pred_prob value from the input
			for (int i = 0; i < row; i++) {
				float c_x = input[i][0];
				float c_y = input[i][1];
				float width = input[i][2];
				float height = input[i][3];
				pred_xywh[i][0] = c_x;
				pred_xywh[i][1] = c_y;
				pred_xywh[i][2] = width;
				pred_xywh[i][3] = height;
			}

			for (int i = 0; i < row; i++) {
				float conf = input[i][4];
				pred_conf[i][0] = conf;
			}

			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col - 5; j++) {
					float prob = input[i][j + 5];
					pred_prob[i][j] = prob;
				}
			}


			//save the boundingbox info into a new matrix
			std::vector<std::vector<float>> pred_coord(row, std::vector<float>(4, 0.0)); //xmin, ymin, xmax, ymax
			for (int i = 0; i < row; i++) {
				float c_x = pred_xywh[i][0];
				float c_y = pred_xywh[i][1];
				float width = pred_xywh[i][2];
				float height = pred_xywh[i][3];
				float minx, miny, maxx, maxy;
				minx = c_x - width / 2;
				miny = c_y - height / 2;
				maxx = c_x + width / 2;
				maxy = c_y + height / 2;
				pred_coord[i][0] = minx;
				pred_coord[i][1] = miny;
				pred_coord[i][2] = maxx;
				pred_coord[i][3] = maxy;
			}


			float resize_ratio = min(float(input_size) / ori_h, float(input_size) / ori_w);
			float dw = float(input_size - resize_ratio * ori_w) / 2;
			float dh = float(input_size - resize_ratio * ori_h) / 2;


			for (int i = 0; i < row; i++) {
				for (int j = 0; j < 4; j += 2) {
					float replace = (pred_coord[i][j] - dw) / resize_ratio;
					pred_coord[i][j] = replace;
				}
			}

			for (int i = 0; i < row; i++) {
				for (int j = 1; j < 4; j += 2) {
					float replace = (pred_coord[i][j] - dh) / resize_ratio;
					pred_coord[i][j] = replace;
				}
			}

			//clip some bboxes which are out of range
			for (int i = 0; i < row; i++) {
				float* x = &pred_coord[i][0];
				float* y = &pred_coord[i][1];
				float* w = &pred_coord[i][2];
				float* h = &pred_coord[i][3];
				if (*x < 0) {
					*x = 0;
				}
				if (*y < 0) {
					*y = 0;
				}
				if (*w > ori_w) {
					*w = ori_w;
				}
				if (*h > ori_h) {
					*h = ori_h;
				}

				if (*x > * w || *y > * h) {
					*x = 0;
					*y = 0;
					*w = 0;
					*h = 0;
				}
			}

			//discard some invalid boxes
			for (int i = 0; i < row; i++) {
				float x = pred_coord[i][0];
				float y = pred_coord[i][1];
				float w = pred_coord[i][2];
				float h = pred_coord[i][3];
				float replace = std::sqrt((w - x) * (h - y));
				if (replace < 0 || replace > 999999999999)
					scale_mask[i][0] = false;
			}

			//discard box whose score is lower than threshold
			for (int i = 0; i < row; i++) {
				float max_prob = 0.0;
				int max_pos = 0;
				for (int j = 0; j < col - 5; j++) {
					float temp = pred_prob[i][j];
					if (temp > max_prob) {
						max_prob = temp;
						max_pos = j;
					}
				}
				classes[i][0] = max_pos;
			}


			for (int i = 0; i < row; i++) {
				float score_bbox = pred_conf[i][0];
				float prob_bbox = pred_prob[i][classes[i][0]];
				if (score_bbox * prob_bbox > score_threshold) {
					score_mask[i][0] = true;
				}
			}


			for (int i = 0; i < row; i++) {
				if (score_mask[i][0] && scale_mask[i][0]) {
					mask[i][0] = true;
				}
			}

			for (int i = 0; i < row; i++) {
				if (mask[i][0]) {
					std::vector<float> temp(6);
					temp[0] = pred_coord[i][0];
					temp[1] = pred_coord[i][1];
					temp[2] = pred_coord[i][2];
					temp[3] = pred_coord[i][3];
					temp[4] = pred_conf[i][0];
					temp[5] = float(classes[i][0]);
					output.push_back(temp);
				}
			}
			return output;
		}
		//***************************************************************************************************************************************
		std::vector<std::vector<float>> AIThread::nms(std::vector<std::vector<float>> input, float iou_threshold) {
			int num_bboxes = input.size();
			std::set<float> all_classes; //hashset to store all classes
			std::vector<std::vector<float>> output;
			for (int i = 0; i < num_bboxes; i++) {

				all_classes.insert(input[i][5]);
			}

			//loop in all classes
			for (std::set<float>::iterator it = all_classes.begin(); it != all_classes.end(); it++) {
				float class_ = *it;
				std::vector<std::vector<float>> cls_bboxes;
				for (int i = 0; i < num_bboxes; i++) {
					if (input[i][5] == class_) {
						std::vector<float> temp(6);
						temp[0] = input[i][0];
						temp[1] = input[i][1];
						temp[2] = input[i][2];
						temp[3] = input[i][3];
						temp[4] = input[i][4];
						temp[5] = input[i][5];
						cls_bboxes.push_back(temp);
					}
				}

				//nms
				while (cls_bboxes.size() > 0) {
					int max_ind = -1;
					float max_conf = 0;
					int num = cls_bboxes.size();
					std::vector<std::vector<float>> other_bboxes;
					for (int i = 0; i < num; i++) {
						float conf = cls_bboxes[i][4];
						if (conf > max_conf) {
							if (max_ind != -1) {
								other_bboxes.push_back(cls_bboxes[max_ind]);
							}
							max_conf = conf;
							max_ind = i;
						}
						else {
							other_bboxes.push_back(cls_bboxes[i]);
						}
					}

					std::vector<float> best_box = cls_bboxes[max_ind];
					cls_bboxes.erase(std::begin(cls_bboxes) + max_ind); //remove the best box in vector
					output.push_back(best_box);
					std::vector<float> iou_res = this->iou_bboxes(best_box, other_bboxes);
					std::vector<float> weights(iou_res.size(), 1.0);
					for (int i = 0; i < iou_res.size(); i++) {
						if (iou_res[i] >= iou_threshold) {
							weights[i] = 0.0;
						}
					}


					//std::vector<std::vector<float>> cls_bboxes_temp = cls_bboxes;
					cls_bboxes.clear();
					for (int i = 0; i < iou_res.size(); i++) {
						other_bboxes[i][4] = other_bboxes[i][4] * weights[i];
						if (other_bboxes[i][4] > 0) {
							cls_bboxes.push_back(other_bboxes[i]);
						}
					}
				}
			}
			return output;
		}
		//***************************************************************************************************************************************
		std::vector<float> AIThread::iou_bboxes(std::vector<float> target_box, std::vector<std::vector<float>> other_bboxes) {
			std::vector<float> iou_res;
			for (int i = 0; i < other_bboxes.size(); i++) {
				std::vector<float> temp = other_bboxes[i];
				iou_res.push_back(this->iou(target_box, temp));
			}
			return iou_res;
		}
		//***************************************************************************************************************************************
		float AIThread::iou(std::vector<float> box1, std::vector<float> box2) {
			float minx1 = box1[0];
			float miny1 = box1[1];
			float maxx1 = box1[2];
			float maxy1 = box1[3];

			float minx2 = box2[0];
			float miny2 = box2[1];
			float maxx2 = box2[2];
			float maxy2 = box2[3];

			float box1_area = (maxx1 - minx1) * (maxy1 - miny1);
			float box2_area = (maxx2 - minx2) * (maxy2 - miny2);

			float union_minx = std::max(minx1, minx2);
			float union_miny = std::max(miny1, miny2);
			float union_maxx = min(maxx1, maxx2);
			float union_maxy = min(maxy1, maxy2);

			float width = std::max(union_maxx - union_minx, float(0.0));
			float height = std::max(union_maxy - union_miny, float(0.0));

			float intersection = width * height;
			float iou_ = intersection / (box1_area + box2_area - intersection);
			return iou_;
		}
		//***************************************************************************************************************************************
		std::vector<std::vector<std::pair<std::vector<int>, float>>> AIThread::Detection_batch(std::vector<float> prob_list, std::vector<std::pair<int, int>> wsi_pos, std::pair<int, int> img_shape, tensorflow::Tensor input_tensor, tensorflow::Session* session_det, std::vector<std::vector<std::vector<float>>> cam_batch) {
			int ori_h = m_height_cls;
			int ori_w = m_width_cls;
			int img_h = img_shape.first;
			int img_w = img_shape.second;
			float h_scale = float(img_h) / (ori_h);
			float w_scale = float(img_w) / (ori_w);
			std::vector<int> ori_shape = { ori_h, ori_w };

			int batchsize = input_tensor.dim_size(0);
			int width = input_tensor.dim_size(1);
			int height = input_tensor.dim_size(2);
			std::string alg_input = m_input_node_det;
			//three feature maps
			std::vector<tensorflow::Tensor> pred_bbox;
			std::vector<std::vector<std::pair<std::vector<int>, float>>> ret;

			//three output node
			std::string output_sbbox = m_output_node_sbbox;
			std::string output_mbbox = m_output_node_mbbox;
			std::string output_lbbox = m_output_node_lbbox;

			//run inference three times
			std::pair<std::string, tensorflow::Tensor>img_feed(alg_input, input_tensor);
			const std::string trainable = "training:0";
			tensorflow::Tensor trainable_tensor(tensorflow::DT_BOOL, tensorflow::TensorShape());
			auto t_trainable = trainable_tensor.scalar<bool>();
			t_trainable(0) = false;
			std::pair<std::string, tensorflow::Tensor>img_feed_2_trainable(trainable, trainable_tensor);
			tensorflow::Status status_1 = session_det->Run({ img_feed, img_feed_2_trainable }, { output_sbbox, output_mbbox, output_lbbox }, {}, &pred_bbox);
			tensorflow::Tensor t_sbbox = pred_bbox[0]; 
			tensorflow::Tensor t_mbbox = pred_bbox[1];
			tensorflow::Tensor t_lbbox = pred_bbox[2];

			/* 这里flat方法是将模型输出的tensor拉成一个B*H*W*C的vector,里面是float数。
			如果我们的batch大小是6，在我们的yolov3模型中输出就是6*H*W*(5+num_classes)长度的vector， H*W是feature map的大小， yolov3有3个不同scale的feature map */
			auto probilities_sbbox = t_sbbox.flat<float>();
			auto probilities_mbbox = t_mbbox.flat<float>();
			auto probilities_lbbox = t_lbbox.flat<float>();


			unsigned int col = 5 + m_num_classes;
			unsigned int row = probilities_sbbox.size() / (col * batchsize) + probilities_mbbox.size() / (col * batchsize) + probilities_lbbox.size() / (col * batchsize);
			std::vector<std::vector<std::vector<float>>> concat(batchsize, std::vector<std::vector<float>>(row, std::vector <float>(col, 0.0)));

			int quantity_per_batch_sbbox = probilities_sbbox.size() / batchsize;
			int quantity_per_batch_mbbox = probilities_mbbox.size() / batchsize;
			int quantity_per_batch_lbbox = probilities_lbbox.size() / batchsize;

			//把vector格式的预测结果转换成matrix格式
			for (int b = 0; b < batchsize; b++) {
				unsigned int j = 0;
				for (unsigned int i = 0; i < (probilities_sbbox.size() / batchsize); i = i + col) {
					for (unsigned int k = 0; k < col; k++) {
						concat[b][j][k] = probilities_sbbox(b * quantity_per_batch_sbbox + i + k);
					}
					j += 1;
				}


				for (unsigned int i = 0; i < (probilities_mbbox.size() / batchsize); i = i + col) {
					for (unsigned int k = 0; k < col; k++) {
						concat[b][j][k] = probilities_mbbox(b * quantity_per_batch_mbbox + i + k);
					}
					j += 1;
				}

				for (unsigned int i = 0; i < (probilities_lbbox.size() / batchsize); i = i + col) {
					for (unsigned int k = 0; k < col; k++) {
						concat[b][j][k] = probilities_lbbox(b * quantity_per_batch_lbbox + i + k);
					}
					j += 1;
				}
			}


			std::vector<std::pair<std::vector<int>, float>> batch_res;
			for (int b = 0; b < batchsize; b++) {
				batch_res.clear();
				auto bboxes = this->postprocess(concat[b], ori_shape, ori_h, m_prob_threshold_det);
				bboxes = this->nms(bboxes, m_iou_threshold);
				for (auto bbox : bboxes) {
					std::vector<int> bboxes_ = { int(bbox[0]), int(bbox[1]), int(bbox[2]), int(bbox[3]) };
					int xmin_temp = int(bbox[0]);
					int ymin_temp = int(bbox[1]);
					int xmax_temp = int(bbox[2]);
					int ymax_temp = int(bbox[3]);
					if (bbox[4] > m_prob_threshold_det_AI && prob_list[b] > m_prob_threshold_cls_AI)
						m_output_TCT->num_bboxes++;
					std::vector<std::vector<float>> cam_input = cam_batch[b];

					//std::pair<int, int> init_pos = init_list[b];
					//float cam_summ = this->summ_cam(this->vector2Mat(cam_input), bboxes_);
					//float ratio = cam_summ * 1.0 / ((xmax_temp - xmin_temp) * (ymax_temp - ymin_temp));
					//std::cout << "cam summ: "<< ratio << std::endl;
					//std::cout << "cam summ: " << cam_summ << std::endl;
					//if (ratio < m_cam_threshold)
						//continue;
					
					//将预测的坐标映射回原图大小
					int xmin = int(int(bbox[0]) * w_scale) + wsi_pos[b].second;
					int ymin = int(int(bbox[1]) * h_scale) + wsi_pos[b].first;
					int xmax = int(int(bbox[2]) * w_scale) + wsi_pos[b].second;
					int ymax = int(int(bbox[3])  * h_scale) + wsi_pos[b].first;
					float score = (bbox[4] + prob_list[b])/2; //这个prob_list里就是这个框对应patch的恶性概率
					int classification = bbox[5];
					std::pair<std::vector<int>, float> temp;
					temp.first = { xmin, ymin, xmax, ymax, classification};
					temp.second = score;
					batch_res.push_back(temp);
				}
				ret.push_back(batch_res);
			}
			return ret;
		}
		//############################################################################################################################################


		void AIThread::run()
		{
			int imgidx_batch = 0;
			std::vector<std::pair<cv::Mat, std::pair<int, int>>> coords;
			std::pair<int, int> img_shape(m_cut_width, m_cut_height);
			img_shape.first = this->m_cut_height;
			img_shape.second = this->m_cut_width;

			//取公有容器中的patch，直到容器容量为0且IOThread切完了所有的图才break
			while (true)
			{
				if (m_io->get_flag() && m_io->get_patch_count() == 0)
				{
					break;
				}

				tensorflow::Tensor input_tensor_cls(tensorflow::DT_FLOAT, tensorflow::TensorShape({ m_batch_size_cls, 3, m_width_cls, m_height_cls }));
				tensorflow::Tensor input_tensor_qc(tensorflow::DT_FLOAT, tensorflow::TensorShape({ m_batch_size_cls, 3, m_width_cls, m_height_cls }));

				//取满一个batch的tensor就去做算法预测部分
				while (imgidx_batch < m_batch_size_cls)
				{

					//if container has nothing but the io thread is not end, continue
					if (m_io->get_patch_count() == 0 && !(m_io->get_flag()))
						continue;

					//if container has nothing and io thread end, break
					if (m_io->get_patch_count() == 0 && m_io->get_flag())
						break;

					if (m_io->get_patch_count() > 0) {
						patch p = m_io->get_patch();
						cv::Mat img = p.array;

						// preprocessing
						img = resize_image(img, m_width_cls, m_height_cls);
						img = normalize_image(img);
						coords.push_back(std::pair<cv::Mat, std::pair<int, int>>(img, p.coord));

						this->Mat2ChannelFirstTensor(img.clone(), input_tensor_cls, imgidx_batch);
						this->Mat2ChannelFirstTensor(img.clone(), input_tensor_qc, imgidx_batch);

						imgidx_batch++;
						image_idx += 1;
						m_output_TCT->image_with_cells += 1;
					}
				}
				//将batch_idx置0，以便下一次batch的计数
				imgidx_batch = 0;
				
				//接下去就是模型预测部分，包括了质量控制和算法模型
				if (m_input_TCT->control) {
					this->Quality_control(m_output_TCT, m_input_TCT, input_tensor_qc, this->session_qc);
					LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "quality control done " << image_idx);
				}

				if (m_input_TCT->algorithm) {
					std::pair<std::vector<float>, float> alg_res = Algorithm(m_output_TCT, m_input_TCT, input_tensor_cls, this->session_cls);
					std::vector<float> probs = alg_res.first;
					LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "classification done " << image_idx);

					std::vector<int> det_index;
					for (int idx = 0; idx < m_batch_size_cls; idx++) {
						if (probs[idx] > m_prob_threshold_cls)
						{
							m_input_TCT->num_tumor_patch += 1;
							std::pair<cv::Mat, std::pair<int, int>> temp(coords[idx].first, coords[idx].second);
							detection_img.push_back(temp);
							cls_probs.push_back(probs[idx]);
							this->cam_ret.push_back(m_output_TCT->cam_ret[idx]);
						}
					}


					coords.clear();
					std::vector<std::vector<std::vector<float>>> cam_batch;
					
					if (detection_img.size() >= m_batch_size_det) {
						tensorflow::Tensor img_tensor_det(tensorflow::DT_FLOAT, tensorflow::TensorShape({ m_batch_size_det, m_width_cls, m_height_cls, 3 }));
						std::vector<std::pair<int, int>> init_pos;
						std::vector<std::pair<int, int>> wsi_pos;
						std::vector<float> prob_list;
						for (int idx_det = 0; idx_det < m_batch_size_det; idx_det++) {
							auto image_det = detection_img.front();
							wsi_pos.push_back(image_det.second);
							cv::Mat input_img = image_det.first;
							cam_batch.push_back(this->cam_ret.front());
							prob_list.push_back(cls_probs.front());
							this->Mat2ChannelLastTensor(input_img.clone(), img_tensor_det, idx_det);
							detection_img.pop_front();
							this->cam_ret.pop_front();
							cls_probs.pop_front();
						}

						LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "start detection" << image_idx);
						std::vector<std::vector<std::pair<std::vector<int>, float>>> bbox_res = this->Detection_batch(prob_list, wsi_pos, img_shape, img_tensor_det, this->session_det, cam_batch);
						LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "detection done" << image_idx);
						wsi_pos.clear();

						//postprocess
						for (int idx = 0; idx < bbox_res.size(); idx++) {
							for (auto bbox : bbox_res[idx]) {
								if (bbox.first[4] == 0)
									lsil_bboxes.push_back(bbox);
								else if (bbox.first[4] == 1)
									hsil_bboxes.push_back(bbox);
							}
						}
					}
				}
			}

				//对可能剩余的数据做detection
				std::vector<std::vector<std::vector<float>>> cam_batch;
				if (detection_img.size() > 0) {
					m_batch_size_det = detection_img.size();
					tensorflow::Tensor img_tensor_det(tensorflow::DT_FLOAT, tensorflow::TensorShape({ m_batch_size_det, m_width_cls, m_height_cls, 3 }));
					std::vector<std::pair<int, int>> init_pos;
					std::vector<std::pair<int, int>> wsi_pos;
					std::vector<float> prob_list;
					for (int idx_det = 0; idx_det < m_batch_size_det; idx_det++) {
						auto image_det = detection_img.front();
						cv::Mat input_img = image_det.first;
						wsi_pos.push_back(image_det.second);
						cam_batch.push_back(this->cam_ret.front());
						prob_list.push_back(cls_probs.front());
						this->Mat2ChannelLastTensor(input_img.clone(), img_tensor_det, idx_det);
						detection_img.pop_front();
						this->cam_ret.pop_front();
					}
					LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "start detection" << image_idx);
					std::vector<std::vector<std::pair<std::vector<int>, float>>> bbox_res = this->Detection_batch(prob_list, wsi_pos, img_shape, img_tensor_det, this->session_det, cam_batch);
					LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "detection done" << image_idx);
					wsi_pos.clear();

					//postprocess
					for (int idx = 0; idx < bbox_res.size(); idx++) {
						for (auto bbox : bbox_res[idx]) {
							if (bbox.first[4] == 0)
								lsil_bboxes.push_back(bbox);
							else if (bbox.first[4] == 1)
								hsil_bboxes.push_back(bbox);
						}
					}
				}

				sort(lsil_bboxes.begin(), lsil_bboxes.end(), cmp);//按照第一个元素排序
				sort(hsil_bboxes.begin(), hsil_bboxes.end(), cmp);//按照第一个元素排序
				
				int num_lsil = 10;
				int num_hsil = 10;
				if (lsil_bboxes.size() < 10)
				    num_lsil = lsil_bboxes.size();
				if (hsil_bboxes.size() < 10)
				    num_hsil = hsil_bboxes.size();
				    
				for (auto i = 0; i < num_lsil; i++) {
					final_bbox_res.push_back(lsil_bboxes[i]);
				}
				
				
				for (auto i = 0; i < num_hsil; i++) {
					final_bbox_res.push_back(hsil_bboxes[i]);
				}

				LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "All images prediction done");

				m_output_TCT->bboxes_res = final_bbox_res;
				float blur_ratio = float(m_output_TCT->blur_images) / m_output_TCT->image_with_cells;
				float zazhi_ratio = float(m_output_TCT->zazhi_images) / m_output_TCT->image_with_cells;
				m_output_TCT->blur_ratio = blur_ratio;
				m_output_TCT->zazhi_ratio = zazhi_ratio;

				//对一些corner case做判断
				if ((lsil_bboxes.size() == 0) && (hsil_bboxes.size() == 0))
					m_output_TCT->disease_type = type0;
				else if (lsil_bboxes.size() > 0 && hsil_bboxes.size() == 0) {
					if (m_output_TCT->num_bboxes > m_num_threshold || lsil_bboxes[0].second > m_cell_prob_threshold) {
						m_output_TCT->sample_result = 1;
						m_output_TCT->disease_type = type1;
					}
					else
						m_output_TCT->disease_type = type0;
				}
				else if (lsil_bboxes.size() == 0 && hsil_bboxes.size() > 0) {
					if (m_output_TCT->num_bboxes > m_num_threshold || hsil_bboxes[0].second > m_cell_prob_threshold) {
						m_output_TCT->sample_result = 1;
						m_output_TCT->disease_type = type1;
					}
					else
						m_output_TCT->disease_type = type0;
				}
				else
				{
					if (m_output_TCT->num_bboxes > m_num_threshold || lsil_bboxes[0].second > m_cell_prob_threshold || hsil_bboxes[0].second > m_cell_prob_threshold) {
						m_output_TCT->sample_result = 1;
						m_output_TCT->disease_type = type1;
					}
					else
						m_output_TCT->disease_type = type0;
				}


				m_io->m_patchList.clear();
				this->cam_ret.clear();
				detection_img.clear();
				m_output_TCT->sample_name = m_input_TCT->m_slide_name;
		}


		//启动线程
		void AIThread::startThread()
		{
			std::thread run_thread(std::bind(&AIThread::run, this));
			run_thread.join();
		}


	}
}