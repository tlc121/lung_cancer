#if !defined(_IFPIPELINEPATHOLOGYTCTOUT_H)
#define _IFPIPELINEPATHOLOGYTCTOUT_H

#include "IFBase.h"
#include <map>
#include <string>
#include <vector>

namespace ALPHA
{
	namespace TCT
	{
		class IFPipelinePathologyTCTOut : public COMM::IFBase {
		public:
			IFPipelinePathologyTCTOut() :COMM::IFBase() {}
			~IFPipelinePathologyTCTOut() override {}
			int task_type = 2;
			int sample_result = 0;
			float cell_ratio = 0;
			std::string disease_type = "Normal";
			std::vector<std::string> Imagelist;
			std::string sample_name;
			int blur_images = 0;
			int zazhi_images = 0;
			float blur_ratio = 0.0;
			float zazhi_ratio = 0.0;
			int image_with_cells = 0;
			int num_bboxes = 0;
			std::vector<std::pair<std::vector<int>, float>> bboxes_res;
			std::vector<std::vector<std::vector<float>>> cam_ret;
			bool all_image_broken = false;
			bool is_empty = false;
		};
	}
}



#endif  //_IFPIPELINEPATHOLOGYTCTOUT_H