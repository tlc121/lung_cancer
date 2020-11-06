#include "AIThread.h"


namespace ALPHA
{
	namespace FEIAI {

		cv::Mat resize_image(const cv::Mat& in_img, const int input_width, const int input_height)
		{
			cv::Size dsize = cv::Size(in_img.cols, in_img.rows);
			cv::Mat out_img = cv::Mat(dsize, CV_8UC3);
			cv::resize(in_img, out_img, cv::Size(input_width, input_height));
			return std::move(out_img);
		}

		cv::Mat remove_bg(const cv::Mat& in_img)
		{
			cv::Size dsize = cv::Size(in_img.cols, in_img.rows);
			cv::Mat out_img = cv::Mat(dsize, CV_8UC3);
			cv::cvtColor(in_img, out_img, cv::COLOR_BGR2Lab);
			int abins = 256, bbins = 256;
			int histSize[] = { abins, bbins };
			float aranges[] = { 0, 256 };
			float branges[] = { 0, 256 };
			const float* ranges[] = { aranges, branges };
			cv::MatND hist;
			int channels[] = { 1, 2 };

			cv::calcHist(&out_img, 1, channels, cv::Mat(),
				hist, 2, histSize, ranges,
				true, // the histogram is uniform
				false);
			cv::Point maxPoint;
			minMaxLoc(hist, 0, 0, 0, &maxPoint);

			int a_offset = maxPoint.y - 128;
			int b_offset = maxPoint.x - 128;

			if (abs(a_offset) < 20 && abs(b_offset) < 20)
			{
				for (int i = 0; i < out_img.rows; i++)
				{
					for (int j = 0; j < out_img.cols; j++)
					{
						cv::Vec3b pixel = out_img.at<cv::Vec3b>(i, j);
						out_img.at<cv::Vec3b>(i, j)[1] = pixel.val[1] - a_offset;
						out_img.at<cv::Vec3b>(i, j)[2] = pixel.val[2] - b_offset;
					}
				}
				cv::cvtColor(out_img, out_img, cv::COLOR_Lab2RGB);
			}
			else
				out_img = in_img;

			return std::move(out_img);
		}

		cv::Mat normalize_image(const cv::Mat& in_img)
		{
			int rows = in_img.rows;
			int cols = in_img.cols;
			cv::Mat norm_img = cv::Mat(rows, cols, CV_32FC3, cv::Scalar::all(0));

			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					cv::Vec3b pixel = in_img.at<cv::Vec3b>(i, j);
					uchar blue = pixel.val[0];
					uchar green = pixel.val[1];
					uchar red = pixel.val[2];

					float replace_b = (blue / 255.0 - 0.5) / 0.1;
					float replace_g = (green / 255.0 - 0.5) / 0.1;
					float replace_r = (red / 255.0 - 0.5) / 0.1;

					float* r = &norm_img.at<cv::Vec3f>(i, j)[0];
					float* g = &norm_img.at<cv::Vec3f>(i, j)[1];
					float* b = &norm_img.at<cv::Vec3f>(i, j)[2];

					*r = replace_r;
					*b = replace_b;
					*g = replace_g;
				}
			}
			return std::move(norm_img);
		}

		void ALPHA::FEIAI::AIThread::startThread()
		{
			std::thread t(std::thread(std::bind(&AIThread::run, this)));
			t.join();
		}

		void ALPHA::FEIAI::AIThread::run()
		{

			// step 0: parse config
			libconfig::Config config;
			COMM::loadCfg(m_configPath, config);
			const libconfig::Setting& root = config.getRoot();
			const libconfig::Setting& shape = root["FEIAI_PREPROCESS"]["CLS_INPUT_SHAPE"];
			const int width = shape[0];
			const int height = shape[1];

			const int batch_size = config.lookup("FEIAI_CLS.BATCH_SIZE");
			const int num_tumor_thres = config.lookup("FEIAI_CLS.NUM_PATCH_THRESHOLD");
			const float tumor_prob_thres = config.lookup("FEIAI_CLS.PROB_CLS_THRESHOLD");
			const std::string model_path_normal_disease = config.lookup("FEIAI_CLS.MODEL_CLS_PATH");
			const std::string input_node_normal_disease = config.lookup("FEIAI_CLS.INPUT_CLS_NODE");
			const std::string output_node_normal_disease = config.lookup("FEIAI_CLS.OUTPUT_CLS_NODE");
			const std::string normal_disease_type_0 = config.lookup("FEIAI_CLS.TYPE_0");
			const std::string normal_disease_type_1 = config.lookup("FEIAI_CLS.TYPE_1");

			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "parse success");

			// step 1: load model
			tensorflow::GraphDef graph_def_normal_disease;
			//tensorflow::GraphDef graph_def_disease_subtype;

			if (!ReadBinaryProto(tensorflow::Env::Default(), model_path_normal_disease, &graph_def_normal_disease).ok())
			{
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "load model .pb failed, check the path.");
			}
			else
			{
				LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "load model successfully!");
			}


			std::unique_ptr<tensorflow::Session> session_normal_disease;
			//std::unique_ptr<tensorflow::Session> session_disease_subtype;
			tensorflow::SessionOptions sess_opt;
			sess_opt.config.mutable_gpu_options()->set_allow_growth(true);

			(&session_normal_disease)->reset(NewSession(sess_opt));
			if (!session_normal_disease->Create(graph_def_normal_disease).ok()) {
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "Create graph failed");
			}


			// slide prediction
			int prediction = 0;
			// the index of image in batch
			int imgidx_batch = 0;
			// the tensor of batch, default size is batchsize
			tensorflow::Tensor input_tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({ batch_size, 3, width, height }));
			// patch coordinates
			std::vector<std::pair<int, int>> coords;
			// outputs of batch
			std::vector<tensorflow::Tensor> outputs;

			//step 2: batch inference
			int batch_index = 0;
			while (true)
			{
				if (m_io->m_flag && m_io->get_patchlist_size() == 0)
				{
					LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "AIThread finish.");
					break;
				}

				
				while (imgidx_batch < batch_size)
				{
					if (m_io->get_patchlist_size() == 0 && !m_io->m_flag)
						continue;

					if (m_io->get_patchlist_size() == 0 && m_io->m_flag)
						break;

					patch p = m_io->get_patch();
					cv::Mat img = p.array;
					coords.push_back(p.coord);
					// preprocessing
					img = resize_image(img, width, height);
					img = normalize_image(img);


					// put image in the batch tensor
					// FEIAI 2 cls input

					//cv::Mat img_(224, 224, CV_32FC3, cv::Scalar::all(1));
					auto input_tensor_mapped = input_tensor.tensor<float, 4>();
					int depth = img.channels();
					const float* data = (float*)img.data;
					for (int y = 0; y < height; y++)
					{
						const float* dataRow = data + (y * width * depth);
						for (int x = 0; x < width; x++)
						{
							const float* dataPixel = dataRow + (x * depth);
							for (int c = 0; c < depth; c++)
							{
								const float* dataValue = dataPixel + c;
								input_tensor_mapped(imgidx_batch, c, y, x) = *dataValue;
							}
						}
					}
					m_output->num_cell_patch++;
					imgidx_batch++;
				}

				std::pair<std::string, tensorflow::Tensor>img_feed(input_node_normal_disease, input_tensor);
				outputs.clear();
				//std::cout << "running model" << std::endl;
				tensorflow::Status status = session_normal_disease->Run({ img_feed }, { output_node_normal_disease }, {}, &outputs);
				//std::cout << "running model done" << std::endl;
				//std::cout << outputs.size() << std::endl;
				auto probilities = outputs[0].matrix<float_t>();

				for (int im_id = 0; im_id < imgidx_batch; im_id++)
				{	
					
					if (probilities(im_id, 1) > tumor_prob_thres)
					{
						//std::cout << probilities(im_id, 1) << std::endl;
						m_output->num_tumor_patch++;
						m_output->bbox_results.push_back({ {coords[im_id].first, coords[im_id].second, coords[im_id].first + m_io->m_cutWidth, coords[im_id].second + m_io->m_cutHeight}, probilities(im_id, 1) });
					}
				}
				imgidx_batch = 0;
				coords.clear();
				batch_index += 1;
				LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "Batch " << batch_index << " finished");
			}

			float tumor_ratio = (float)m_output->num_tumor_patch / this->m_io->m_num_images;
			std::cout << tumor_ratio << std::endl;
			// step 3: result
			if (m_output->num_tumor_patch > num_tumor_thres)
			{
				prediction = 1;
				m_output->sample_result = "Malignancy";
			}

			m_output->sample_name = m_io->m_slide_name;
			m_output->num_blur_patch = m_io->m_num_blur;
			m_output->ratio_blur = m_io->m_ratio_blur;
			//m_output->num_cell_patch = m_io->m_num_images;
			m_output->num_cell_patch = m_io->m_num_images;
			m_output->num_bbox = m_output->num_tumor_patch;
		}
	}
}