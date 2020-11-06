#include <iostream>
#include <memory>
#include "InferLocalPathologyPDL1.h"
#include "IFPipelinePathologyPDL1In.h"
#include "IFPipelinePathologyPDL1Out.h"
#include "PipelinePathologyPDL1.h"
#include <time.h>






int main()
{
	std::cout << "-------test------" << std::endl;
	std::string image_folder = "D:\\data\\PDL1\\2M05_1_2\\";
    std::string config_path = "D:\\tlc\\PDL1\\4.39\\alpha_bk\\configuration\\pathology_config.cfg";

	std::shared_ptr<ALPHA::PDL1::IFPipelinePathologyPDL1In> ptrPipelineInput(new ALPHA::PDL1::IFPipelinePathologyPDL1In(image_folder, config_path));
	std::shared_ptr<ALPHA::PDL1::PipelinePathologyPDL1> pipeline(new ALPHA::PDL1::PipelinePathologyPDL1(config_path));

	pipeline->read_seg_model();

	pipeline->setInput(ptrPipelineInput);
	
	clock_t t_update_start = clock();
	pipeline->update();
	std::cout << "alg takes: " << (clock() - t_update_start) * 1.0 / CLOCKS_PER_SEC << "s" << std::endl;

	std::shared_ptr<ALPHA::COMM::DataObj> ptroutput = pipeline->getOutput();
	std::shared_ptr<ALPHA::PDL1::IFPipelinePathologyPDL1Out> poutput = std::dynamic_pointer_cast<ALPHA::PDL1::IFPipelinePathologyPDL1Out>(ptroutput);
	std::cout << "name: " << ptrPipelineInput->m_slide_name << std::endl;
	std::cout << "result: " << poutput->sample_result << std::endl;
	std::cout << "patch_info: " << poutput->sample_details[0].image_name << std::endl;
	std::cout << "tps_ratio: " << poutput->tps_ratio << std::endl;

	for (auto patchinfo : poutput->sample_details) {
		cv::Mat ori_image;
		std::string img_name = image_folder + "Images\\" + patchinfo.image_name;
		std::cout << img_name << std::endl;
		ori_image = cv::imread(img_name);
		std::vector<std::vector<cv::Point>> contours_draw;
		std::vector<std::vector<std::vector<std::pair<int, int>>>> contours = patchinfo.contour;
		std::vector<std::vector<std::pair<int, int>>> contour_int = contours[0];
		for (std::vector<std::pair<int, int>> point_set: contour_int) {
			std::vector<cv::Point> point_set_temp;
			for (std::pair<int, int> point_temp : point_set) {
				int x = point_temp.first;
				int y = point_temp.second;
				cv::Point s(x, y);
				point_set_temp.push_back(s);
			}
			contours_draw.push_back(point_set_temp);
		}
		cv::drawContours(ori_image, contours_draw, -1, cv::Scalar(255, 0, 0));
		cv::imshow("draw", ori_image);
		cv::waitKey();
	}
	return 0;
}