#ifndef _IO_THREAD_H_
#define _IO_THREAD_H_

#include <thread>
#include <mutex>
#include <list>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <omp.h>
#include <fstream>
#include <iostream>
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
#include "IFPipelinePathologyLungCancerOUT.h"
#include "IFPipelinePathologyLungCancerIn.h"
#include <stdint.h>
#include "openslide-features.h"
#include "openslide.h"
//#include "InferLocalPathologyLungCancer.h"


namespace ALPHA
{
	namespace LungCancer
	{
		struct patch {
			std::pair<int, int> coord;
			cv::Mat array;
		};

		class IOThread
		{
		public:
			explicit IOThread() {}
			explicit IOThread(IFPipelinePathologyLungCancerIn* LungCancer_input, openslide_t* slide, int cut_width, int cut_height) : m_LungCancer_input(LungCancer_input), m_slide(slide), m_cutWidth(cut_width), m_cutHeight(cut_height) { m_slide_name = m_LungCancer_input->m_slide_name; }
			~IOThread() {}
			patch get_patch();
			void startThread();
			int get_patchlist_size();

			std::string m_slide_name;
			int m_cutWidth = 0;
			int m_cutHeight = 0;
			int m_num_images = 0;
			int m_num_blur = 0;
			int m_num_blank = 0;
			int m_num_broken = 0;
			float m_ratio_blur = 0.;
			std::atomic<bool> m_flag = false; // flag of run status
			bool m_svsBroken = false;
			bool m_formatDismatch = false;
			openslide_t* m_slide;
			std::thread* m_run_thread;
			//int m_num_patches = 0;

		private:
			IFPipelinePathologyLungCancerIn* m_LungCancer_input;
			std::mutex m_mutex;
			std::list<patch> m_patchList;

			void add_patch(patch&& p);
			void run();
		};
	}
}
#endif