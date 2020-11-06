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
#include "IFPipelinePathologyTCTOUT.h"
#include "IFPipelinePathologyTCTIn.h"
#include <stdint.h>
#include "openslide-features.h"
#include "openslide.h"
//#include "InferLocalPathologyTCT.h"


namespace ALPHA
{
	namespace TCT {

		struct patch {
			std::pair<int, int> coord;
			cv::Mat array;
		};


		class IoThread
		{
		private:
			cv::Mat read_patch(openslide_t* input, int x_start, int y_start, int width, int height, int level);
			float cal_var(std::vector<int> im_flat);
			bool stop_flag = false;
			void add_patch(patch img);
			void run();

		public:
			explicit IoThread();
			explicit IoThread(openslide_t* slide, int cut_width, int cut_height, int start_pos_h, int start_pos_w);
			~IoThread() {}
			static std::mutex m_mutex;
			static std::list<patch> m_patchList;
			static bool pause_flag;
			patch get_patch();
			int get_patch_count() {
				m_mutex.lock();
				auto patch_num = m_patchList.size();
				m_mutex.unlock();
				return patch_num;
			}
			void pause();
			void start();

			bool get_flag() {
				return stop_flag;
			}
			void startThread();
			std::thread* m_run_thread;

			//bool get_patch(patch* patch_img);
			openslide_t* slide;
			int cut_width;
			int cut_height;
			int m_start_pos_h;
			int m_start_pos_w;
			int m_width_num;
			int m_height_num;
		};
	}
}
#endif