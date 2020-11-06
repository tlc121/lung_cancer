#include "AIThread.h"
cv::Mat resize_image(const cv::Mat& in_img, const int input_width, const int input_height)
{
	cv::Size dsize = cv::Size(in_img.cols, in_img.rows);
	cv::Mat out_img = cv::Mat(dsize, CV_8UC3);
	cv::resize(in_img, out_img, cv::Size(input_width, input_height));
	return std::move(out_img);
}


void move(std::vector<std::vector<int>>& x, std::vector<std::vector<int>>& y, int i, int j, int& num2, int& sum2, int& xmin, int& xmax, int& ymin, int& ymax, int num, int width, int height,int s0)
{
	int path[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
	for (int i0 = 0; i0 < 4; i0++)
	{
		int i1 = path[i0][0] + i;
		int j1 = path[i0][1] + j;
		int step = 0;
		while  ((0 <= i1) && (i1 < height) && (0 <= j1) && (j1 < width))
		{
			if (y[i1][j1] == 0)
			{
				step += 1;
				if (i1 < xmin)
					xmin = i1;
				if (i1 > xmax)
					xmax = i1;
				if (j1 < ymin)
					ymin = j1;
				if (j1 > ymax)
					ymax = j1;
				y[i1][j1] = num;
				sum2 += 1;
				if (x[i1][j1] == s0)
					num2 += 1;
				i1 += path[i0][0];
				j1 += path[i0][1];
			}
			else
				break;
				
		}
		if (step >0)
			for (int step0=1;step0<=step;step0++)
				move(x, y, i + step0 * path[i0][0], j + step0 * path[i0][1], num2, sum2, xmin, xmax, ymin, ymax, num, width, height,s0);
	}
};
std::vector<std::vector<int>> kmeans_det(std::vector<std::vector<int>>& x, std::vector<std::vector<int>>& y, int width, int height)
{
	int num = 0;
	std::vector<std::vector<int>> det;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			if (y[row][col] == 0)
			{
				num += 1;
				y[row][col] = num;
				int num2 = 0, sum2 = 1, xmin = row, xmax = row, ymin = col, ymax = col;
				move(x, y, row, col, num2, sum2, xmin, xmax, ymin, ymax, num, width, height,3);
				if ((num2 > 50) && ((sum2 * 0.2) < num2))
				{
					det.push_back({ xmin,xmax,ymin,ymax });
				}

			}
		}
	}
	return det;
}
float dlt_sum(std::vector<float> dlt)
{
	float s = 0;
	int n = dlt.size();
	for (int i = 0; i < n; i++)
	{
		s = s + std::abs(dlt[i]);
	}
	return s;
}
std::vector<std::vector<int>> detector(cv::Mat img)
{
	const int width = img.cols;
	const int height = img.rows;
	std::vector<std::vector<int>> x(height, std::vector<int>(width));
	std::vector<std::vector<int>> y(height, std::vector<int>(width));
	int c = img.channels();
	int wh = width * height;
	int clsnum = 4;
	cv::Mat point(wh, c, CV_32F, cv::Scalar(10));
	cv::Mat label;
	cv::Mat lm;
	cv::Mat center(clsnum, 1, point.type());
	int index = 0;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			index = row * width + col;
			cv::Vec3b bgr = img.at<cv::Vec3b>(row, col);
			point.at<float>(index, 0) = static_cast<int>(bgr[0]);
			point.at<float>(index, 1) = static_cast<int>(bgr[1]);
			point.at<float>(index, 2) = static_cast<int>(bgr[2]);
		}
	}
	cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 0.1);
	cv::kmeans(point, clsnum, label, criteria, 3, cv::KMEANS_PP_CENTERS, center);
	cv::reduce(center, lm, -1, cv::REDUCE_SUM);
	cv::Mat  IndexMat;
	cv::sortIdx(lm, IndexMat, cv::SORT_ASCENDING + cv::SORT_EVERY_COLUMN);
	std::vector<std::vector<int>> det_ans;
	std::vector<float> dlt = { center.at<float>(IndexMat.at<int>(clsnum - 1),0)- center.at<float>(IndexMat.at<int>(0),0),
		center.at<float>(IndexMat.at<int>(clsnum - 1),1) - center.at<float>(IndexMat.at<int>(0),1),
		center.at<float>(IndexMat.at<int>(clsnum - 1),2) - center.at<float>(IndexMat.at<int>(0),2) };
	if ((dlt_sum(dlt) > 300)&&(center.at<float>(IndexMat.at<int>(0), 0)> center.at<float>(IndexMat.at<int>(0), 2)-20))
	{
		std::vector<int> l(4);
		for (int i = 0; i < clsnum; i++)
		{
			l[IndexMat.at<int>(i)] = clsnum - 1 - i;
		}
		int sum3 = 0;
		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				index = row * width + col;
				int lb = label.at<int>(index);
				x[row][col] = l[lb];
				if ((l[lb] == 0) || (l[lb] == 1))
					y[row][col] = -1;
				else
					y[row][col] = 0;
			}
		}
		det_ans = kmeans_det(x, y, width,height);
		
	}
	return det_ans;
}
bool kmeans_pre(std::vector<std::vector<int>>&img0,cv::Mat img, std::vector < std::vector<float>> &color)
{
	const int width = img.cols;
	const int height = img.rows;
	std::vector<std::vector<int>> x(height, std::vector<int>(width));
	std::vector<std::vector<int>> y(height, std::vector<int>(width));
	int c = img.channels();
	int wh = width * height;
	int clsnum = 3;
	cv::Mat point(wh, c, CV_32F, cv::Scalar(10));
	cv::Mat label;
	cv::Mat lm;
	cv::Mat center(clsnum, 1, point.type());
	int index = 0;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			index = row * width + col;
			cv::Vec3b bgr = img.at<cv::Vec3b>(row, col);
			point.at<float>(index, 0) = static_cast<int>(bgr[0]);
			point.at<float>(index, 1) = static_cast<int>(bgr[1]);
			point.at<float>(index, 2) = static_cast<int>(bgr[2]);
		}
	}
	cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 0.1);
	cv::kmeans(point, clsnum, label, criteria, 3, cv::KMEANS_PP_CENTERS, center);
	cv::reduce(center, lm, -1, cv::REDUCE_SUM);
	cv::Mat  IndexMat;
	cv::sortIdx(lm, IndexMat, cv::SORT_ASCENDING + cv::SORT_EVERY_COLUMN);
	std::vector<std::vector<int>> det_ans;
	std::vector<float> dlt = { center.at<float>(IndexMat.at<int>(clsnum - 1),0) - center.at<float>(IndexMat.at<int>(0),0),
		center.at<float>(IndexMat.at<int>(clsnum - 1),1) - center.at<float>(IndexMat.at<int>(0),1),
		center.at<float>(IndexMat.at<int>(clsnum - 1),2) - center.at<float>(IndexMat.at<int>(0),2) };
	for (int i=0 ;i<3;i++)
		color.push_back({ center.at<float>(IndexMat.at<int>(i), 0),center.at<float>(IndexMat.at<int>(i), 1),center.at<float>(IndexMat.at<int>(i), 2) });
	if (dlt_sum(dlt) > 250) //&& (center.at<float>(IndexMat.at<int>(0), 0) > center.at<float>(IndexMat.at<int>(0), 2) - 20))
	{
		std::vector<int> l(clsnum);
		for (int i = 0; i < clsnum; i++)
		{
			l[IndexMat.at<int>(i)] = clsnum - 1 - i;
		}
		int sum3 = 0;
		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				index = row * width + col;
				int lb = label.at<int>(index);
				x[row][col] = l[lb];
				if ((l[lb] == 0))
					y[row][col] = -1;
				else
					y[row][col] = 0;
			}
		}
		int num = 0;
		std::vector<bool> have2;
		int r = 128;
		for (int row = r; row < height-r; row++)
		{
			for (int col = r; col < width-r; col++)
			{
				if (y[row][col] == 0)
				{
					num += 1;
					y[row][col] = num;
					int num2 = 0, sum2 = 1, xmin = row, xmax = row, ymin = col, ymax = col;
					move(x, y, row, col, num2, sum2, xmin, xmax, ymin, ymax, num, width, height, 2);
					if ((num2 > 500) && ((sum2 * 0.2) < num2))
						have2.push_back(TRUE);
					else
						have2.push_back(FALSE);

				}
			}
		}
		if (std::find(have2.begin(), have2.end(), TRUE) != have2.end())
		{
			for (int row = 0; row < height; row++)
			{
				for (int col = 0; col < width; col++)
				{
					if (y[row][col] > 0)
					{
						if (have2[int(y[row][col] - 1)] == FALSE)
							x[row][col] = 0;
					}
					else
						x[row][col] = 0;
					
				}
			}
			img0 = x;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}
bool in_center(std::vector<std::vector<int>> &img0, int i0, int j0,int r0)
{
	int i1 = i0 / 2;
	int j1 = j0 / 2;
	int num = 0;
	for (int i=i1-r0;i<i1+r0;i++)
		for (int j = j1 - r0; j < j1 + r0; j++)
		{
			if (img0[j][i] > 1)
			{
				num += 1;
			}
		}
	if (num * 4 > r0 * r0)
		return TRUE;
	else
		return FALSE;
}
std::vector<std::vector<float>> get_mean_std(std::vector<std::vector<std::vector<float>>> data)
{
	int n = data.size();
	int m = 3;
	std::vector<std::vector<float>> mean_std;
	for (int k = 0; k < m; k++)
	{
		std::vector<float> m_s;
		for (int i = 0; i < m; i++)
		{
			float sum = 0;
			float sum2 = 0;
			for (int j = 0; j < n; j++)
			{
				sum += data[j][k][i];
			}
			for (int j = 0; j < n; j++)
			{
				sum2 += (data[j][k][i] - sum / n) * (data[j][k][i] - sum / n);
			}
			m_s.push_back(sum / n);
			m_s.push_back(sqrt(sum2 / n));
		}
		mean_std.push_back(m_s);
	}
	return mean_std;
}
std::vector<float> z_score(std::vector<std::vector<float>> mean_std, std::vector < std::vector<float>> color)
{
	int m = mean_std.size();
	std::vector<float> z;
	for (int i = 0; i < m; i++)
	{
		float score = 0;
		for (int j = 0; j < 3; j++)
		{
			float s = (color[i][j] - mean_std[i][2*j]) / mean_std[i][2*j+1];
			score += s * s;
		}
		z.push_back(score);
	}
	return z;

}
std::vector < std::vector < std::vector<float>>> get_input(std::vector<std::vector<int>>& img0, cv::Mat img)
{
	const int cols = img.cols;
	const int rows = img.rows;
	std::vector < std::vector < std::vector<float>>> out_img(cols, std::vector<std::vector<float>>(rows, std::vector<float>(3)));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cv::Vec3b pixel = img.at<cv::Vec3b>(i, j);
			int red = pixel[0];
			int green = pixel[1];
			int blue = pixel[2];
			float replace_r = 1.0;
			float replace_g = 1.0;
			float replace_b = 1.0;
			if (img0[i][j] > 0)
			{
				replace_r = float(red / 255.0);
				replace_g = float(green / 255.0);
				replace_b = float(blue / 255.0);
			}
			out_img[i][j][0] = replace_r;
			out_img[i][j][1] = replace_g;
			out_img[i][j][2] = replace_b;
		}
	}
	return out_img;
}
void ALPHA::ROSE::AIThread::start()
{
	std::thread* t = new std::thread(std::thread(std::bind(&AIThread::run, this)));
	m_run_thread = t;
}

void ALPHA::ROSE::AIThread::wait()
{
	m_run_thread->join();
}

void ALPHA::ROSE::AIThread::run()
{
	// step 0: parse config
	libconfig::Config config;
	COMM::loadCfg(m_configPath, config);
	const libconfig::Setting& root = config.getRoot();
	// 2 cls
	const int batch_size = config.lookup("ROSE_CLS_NORMAL_DISEASE.BATCH_SIZE");
	const int num_tumor_thres = config.lookup("ROSE_CLS_NORMAL_DISEASE.NUM_TUMOR_THRESHOLD");
	const float tumor_prob_thres = config.lookup("ROSE_CLS_NORMAL_DISEASE.TUMOR_PROB_THRESHOLD");
	const float tumor_ratio_thres = config.lookup("ROSE_CLS_NORMAL_DISEASE.TUMOR_RATIO_THRESHOLD");
	const std::string model_path_2cls = config.lookup("ROSE_CLS_NORMAL_DISEASE.MODEL_PATH");
	const std::string input_node_2cls = config.lookup("ROSE_CLS_NORMAL_DISEASE.INPUT_NODE");
	const std::string output_node_2cls = config.lookup("ROSE_CLS_NORMAL_DISEASE.OUTPUT_NODE");
	const std::string normal_disease_type_0 = config.lookup("ROSE_CLS_NORMAL_DISEASE.TYPE_0");
	const std::string normal_disease_type_1 = config.lookup("ROSE_CLS_NORMAL_DISEASE.TYPE_1");
	

	LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "parse success");

	// step 1: load model
	tensorflow::GraphDef graph_def_2cls;

	if (!ReadBinaryProto(tensorflow::Env::Default(), model_path_2cls, &graph_def_2cls).ok())
	{
		LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "load model .pb failed, check the path.");
		std::cout << "read model fail!!!!!!!!!!!!check path!!!!!!!!!!!!!!11" << std::endl;
	}
	else
	{
		LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "load model successfully!");
		std::cout << "read model success!!!!!!!!!!!!"  << std::endl;
	}

	std::unique_ptr<tensorflow::Session> session_2cls;
	tensorflow::SessionOptions sess_opt;
	sess_opt.config.mutable_gpu_options()->set_allow_growth(true);

	(&session_2cls)->reset(NewSession(sess_opt));
	if (!session_2cls->Create(graph_def_2cls).ok()) {
		LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "Create graph failed");
	}
	int prediction = 0;
	int imgidx_batch = 0;
	std::vector<tensorflow::Tensor> outputs;
	int img_num = 0;
	std::vector<std::vector<int>> patch_res,patch_res3;

	//step 2: kmeans_det
	std::cout << "inference begin !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << m_io->m_slide_name <<std::endl;
	while (true)
	{
		if ((*m_io_complete_flag) && m_io->get_patchlist_size() == 0)
		{
			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "AIThread finish.");
			break;
		}

		while (imgidx_batch < batch_size)
		{
			//LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, *m_io_complete_flag);
			if (m_io->get_patchlist_size() == 0 && !(*m_io_complete_flag))
			{
				::Sleep(1);
				continue;
			}

			if (m_io->get_patchlist_size() == 0 && (*m_io_complete_flag)) {
				break;
			}
			patch p = m_io->get_patch();
			img_num++;
			cv::Mat img = p.array;
			std::vector<std::vector<int>> bboxes = detector(img);
			//cv::imwrite("D:/zhangxia/result/orig_batch/out_" + m_io->m_slide_name+"_"+std::to_string(img_num) + ".png", img);
			int bs = bboxes.size();
			if (bs == 0)
			{
				imgidx_batch += 1;
				continue;
			}
			for (int jb = 0; jb < bs; jb++)
			{
				int res_pos = jb;
				auto bbox = bboxes[res_pos];
				bool legal = TRUE;
				if ((bbox[1] - bbox[0] > 100) || (bbox[3] - bbox[2] > 100))
					legal = FALSE;
				//if ((bbox[1] - bbox[0] < 16) && (bbox[3] - bbox[2] < 16))
					//legal = FALSE;
				if (legal)
				{
					patch_res3.push_back(bbox);
					std::vector<int> box(4);
					box[0] = int(bbox[2] * 8) + p.coord.first;
					box[1] = int(bbox[0] * 8) + p.coord.second;
					box[2] = int(bbox[3] * 8) + p.coord.first;
					box[3] = int(bbox[1] * 8) + p.coord.second;
					int centerx = (box[0] + box[2]) / 2;
					int centery = (box[1] + box[3]) / 2;
					//if ((centerx > 128) && (centery > 128) && (centerx + 128 < m_io->width*8) && (centery + 128 < m_io->height*8))
					patch_res.push_back(box);
				}
			}
		//»­¿ò
			//int bs0 = patch_res3.size();
			//for (int roi_id = 0; roi_id < bs0; roi_id++)
			//{
			//	int res_pos = roi_id;
			//	cv::rectangle(img, cv::Point(patch_res3[res_pos][2], patch_res3[res_pos][0]), cv::Point(patch_res3[res_pos][3], patch_res3[res_pos][1]), cv::Scalar(255, 0, 0), 1, 1, 0);
			//}
			patch_res3.clear();
			imgidx_batch++;
		}
		imgidx_batch = 0;
	}
	int bbox_num = patch_res.size();
	//int bbox_num = 20;
	std::cout << "bbox_num==========================" << bbox_num << std::endl;
	std::vector<std::vector < std::vector < std::vector<float>>>> img_list;
	std::vector < std::vector<std::vector<float>>> color_list;
	std::vector<std::vector<int>> bbox_det;
	for (int i = 0; i < bbox_num; i++)
	{
		auto box = patch_res[i];
		int centerx = (box[0] + box[2]) / 2;
		int centery = (box[1] + box[3]) / 2;
		int r = 256;
		cv::Mat img = m_io->readPatch(centerx - r, centery - r, 2*r, 2*r, 0);
		std::vector<std::vector<int>> img0;
		std::vector < std::vector<float>> color;
		bool legal=kmeans_pre(img0, img, color);
		if ((legal)&&(in_center(img0,512,512,64)))
		{
			//cv::imwrite("E:/tzb/result/in_" + m_io->m_slide_name + "_" + std::to_string(img_list.size()) + ".png", img);
			std::vector < std::vector < std::vector<float>>> input_img = get_input(img0, img);
			img_list.push_back(input_img);
			bbox_det.push_back(box);
			color_list.push_back(color);
		}	

	}
	std::vector<std::vector<float>> mean_std;
	mean_std=get_mean_std(color_list);
	int i = 0;
	std::vector<std::vector < std::vector < std::vector<float>>>>::iterator i0;
	std::vector<std::vector<int>>::iterator i1;
	std::vector < std::vector<std::vector<float>>>::iterator i2;
	i0 = img_list.begin();
	i1 = bbox_det.begin();
	i2 = color_list.begin();
	while (i < color_list.size())
	{
		std::vector<float> z = z_score(mean_std, color_list[i]);
		if ((z[0] > 4)|| (z[1] > 4)|| (z[2] > 4))
		{
			img_list.erase(i0);
			bbox_det.erase(i1);
			color_list.erase(i2);
		}
		else
		{
			i++;
			i0++;
			i1++;
			i2++;
		}

	}
	int cls_batch_size = 16;
	bbox_num = img_list.size();
	std::cout << "bbox_num==========================" <<bbox_num<< std::endl;
	std::vector<std::vector<int>> tumor_bbox;
	if (bbox_num < num_tumor_thres)
		m_output->sample_result = "Normal";
	else
	{
		int cls_num = bbox_num/cls_batch_size;
		if (bbox_num % cls_batch_size > 0)
			cls_num++;
		for (int i = 0; i < cls_num; i++)
		{
			int cls_batch_size0;
			if ((i + 1) * cls_batch_size <= bbox_num)
				cls_batch_size0 = cls_batch_size;
			else
				cls_batch_size0 = bbox_num - i * cls_batch_size;
			std::cout << "cls_batch_size0===" << cls_batch_size0 << std::endl;
			tensorflow::Tensor cls_tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({ cls_batch_size0, 512, 512, 3 }));
			std::pair<std::string, tensorflow::Tensor>img_feed_3;
			img_feed_3 = make_pair(input_node_2cls, cls_tensor);
			auto cls_tensor_mapped = cls_tensor.tensor<float, 4>();
			
			for (int j = 0; j < cls_batch_size0; j++)
			{
				int num_id = i * cls_batch_size + j;
				for (int y = 0; y < 512; y++)
				{
					for (int x = 0; x < 512; x++)
					{
						for (int c = 0; c < 3; c++)
						{
							// bgr->rgb
							cls_tensor_mapped(j, y, x, 2 - c) = img_list[num_id][y][x][c];
						}
					}
				}
			}

			tensorflow::Status status_cls2 = session_2cls->Run({ img_feed_3 }, { output_node_2cls }, {}, &outputs);
			auto probilities = outputs[0].matrix<float_t>();
			for (int j = 0; j < cls_batch_size0; j++)
			{
				int num_id = i * cls_batch_size + j;
				//int cls_out = 0;
				//cv::Mat img = m_io->readPatch(bbox_det[num_id][0], bbox_det[num_id][1], bbox_det[num_id][2] - bbox_det[num_id][0], bbox_det[num_id][3] - bbox_det[num_id][1], 0);
				//cv::imwrite("E:/tzb/result/out2_" + m_io->m_slide_name + "_" + std::to_string(num_id) + "_" + std::to_string(bbox_det[num_id][0]) + "_" + std::to_string(bbox_det[num_id][1]) + "_" 
					//+ std::to_string(probilities(j, 0)) + ".png", img);
				if (probilities(j, 0) > tumor_prob_thres)
				{
					m_output->bbox_results.push_back({ {int(bbox_det[num_id][0]), int(bbox_det[num_id][1]), int(bbox_det[num_id][2]), int(bbox_det[num_id][3])}, probilities(j, 0) });
					m_output->num_tumor_patch++;
					//cls_out = 1;
					//std::cout << "tumor" << std::endl;
					//std::vector<float> z = z_score(mean_std, color_list[num_id]);
					//std::cout << z[0]<<","<<z[1]<<","<<z[2]<< std::endl;
					//std::cout <<  m_io->m_slide_name <<"_" << std::to_string(num_id) << "_" << std::to_string(cls_out) << ".png" << std::endl;
					//cv::Mat img = m_io->readPatch(bbox_det[num_id][0], bbox_det[num_id][1], bbox_det[num_id][2] - bbox_det[num_id][0], bbox_det[num_id][3] - bbox_det[num_id][1], 0);
					//cv::imwrite("E:/tzb/result/out2_" + m_io->m_slide_name + "_" + std::to_string(num_id)  + ".png", img);
				}
				
			}
		}

		if (m_output->num_tumor_patch >= num_tumor_thres)
			m_output->sample_result = "Malignancy";
		else
			m_output->sample_result = "Normal";
	}
	// step 3: result
	m_output->sample_name = m_io->m_slide_name;
	m_output->num_blur_patch = m_io->m_num_blur;
	m_output->ratio_blur = m_io->m_num_blur * 1.0 / img_num;
	m_output->num_cell_patch = img_num;
	m_output->num_bbox = m_output->num_tumor_patch;
}