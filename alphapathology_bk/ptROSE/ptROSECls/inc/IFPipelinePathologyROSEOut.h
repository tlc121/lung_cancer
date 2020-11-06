//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : ROSE
//  @ File Name : IFPipelinePathologyROSEOut.h
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//


#if !defined(_IFPIPELINEPATHOLOGYROSEOUT_H)
#define _IFPIPELINEPATHOLOGYROSEOUT_H

#include "IFBase.h"
#include <map>
#include <string>
#include <vector>
#include "PipelinePathologyROSE_export.h"

namespace ALPHA
{
	namespace ROSE
	{
		struct PatchInfo {
			std::string image_name;
			std::string sample_name;
			std::string disease_type = "NORMAL";
			std::map<std::string, float> image_details = { { "type" , 0 }, { "prob", 1.0 } };
			std::map<std::string, float> image_details_verbose = { { "NORMAL" , 1.0 }, { "TUMOR", 0.0 }, { "LUAD", 0.0 }, { "LUSC", 0.0 },
																   { "SCLC", 0.0 }, { "NSCLC-NOS", 0.0 }, { "OTHERS", 0.0 } };
		};

		class PIPELINEPATHOLOGYROSE_EXPORT IFPipelinePathologyROSEOut : public COMM::IFBase {
		public:
			IFPipelinePathologyROSEOut() :COMM::IFBase() {}
			~IFPipelinePathologyROSEOut() override {}
			std::vector<PatchInfo> sample_details;
			int task_type = 1;
			int num_tumor_patch = 0;
			int num_broken_patch = 0;
			std::string sample_result = "NORMAL";
			float cell_ratio = 0.0;
			bool all_image_broken = false;
			std::string sample_name;
			std::map<std::string, float> patch_counter = { { "NORMAL", 0 }, { "TUMOR", 0 }, { "LUAD", 0 }, { "LUSC", 0 },
														   { "SCLC", 0 }, { "NSCLC-NOS", 0 }, { "OTHERS", 0 }, { "BROKEN", 0} };

			// svs
			int num_cell = -1;
			int num_bbox = 0;
			int num_cell_patch = 0;
			int num_blur_patch = 0;
			int num_impurity_patch = -1;
			float ratio_blur = 0.;
			float ratio_impurity = -1;
			bool format_wrong = false;
			bool file_broken = false;
			std::vector<std::pair<std::vector<int>, float>> bbox_results;
		};
	}
}



#endif  //_IFPIPELINEPATHOLOGYROSEOUT_H