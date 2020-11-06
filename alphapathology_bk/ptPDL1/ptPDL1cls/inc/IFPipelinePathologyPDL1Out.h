#if !defined(_IFPIPELINEPATHOLOGYPDL1OUT_H)
#define _IFPIPELINEPATHOLOGYPDL1OUT_H

#include "IFBase.h"
#include <map>
#include <string>
#include <vector>

namespace ALPHA
{
	namespace PDL1
	{
		struct PatchInfo {
			std::string image_name;
			std::string sample_name;
			float ratio = 0.0;
			std::vector<std::vector<std::vector<std::pair<int, int>>>> contour;
		};

		class IFPipelinePathologyPDL1Out : public COMM::IFBase {
		public:
			IFPipelinePathologyPDL1Out() :COMM::IFBase() {}
			~IFPipelinePathologyPDL1Out() override {}

			std::vector<PatchInfo> sample_details;
			int task_type = 2;
			int sample_result = 0;
			float tps_ratio = 0;
			std::vector<std::string> Imagelist;
			std::string sample_name;
			bool all_image_broken = false;
		};
	}
}



#endif  //_IFPIPELINEPATHOLOGYPDL1OUT_H