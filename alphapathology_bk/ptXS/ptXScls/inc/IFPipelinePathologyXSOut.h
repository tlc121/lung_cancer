#if !defined(_IFPIPELINEPATHOLOGYXSOUT_H)
#define _IFPIPELINEPATHOLOGYXSOUT_H

#include "IFBase.h"
#include <map>
#include <string>
#include <vector>

namespace ALPHA
{
	namespace XS
	{

		class IFPipelinePathologyXSOut : public COMM::IFBase {
		public:
			IFPipelinePathologyXSOut() :COMM::IFBase() {}
			~IFPipelinePathologyXSOut() override {}


			std::string sample_name;
			std::string sample_result = "Normal";
			int num_cell = -1;
			int num_bbox = 0;
			int task_type = 4;
			int num_cell_patch = 0;
			int num_blur_patch = 0;
			float cell_ratio = 0.0;
			int num_impurity_patch = -1;
			int num_tumor_patch = 0;
			float ratio_blur = 0.;
			float ratio_impurity = -1;
			bool format_wrong = false;
			bool file_broken = false;
			std::vector<std::pair<std::vector<int>, float>> bbox_results;
		};
	}
}



#endif  //_IFPIPELINEPATHOLOGYXSOUT_H