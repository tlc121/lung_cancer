#if !defined(_IOTHREAD_H)
#define _IOTHREAD_H

#include <thread>
#include <mutex>
#include <list>
#include "CommonUtils.h"
#include "opencv2/opencv.hpp"
#include "IFPipelinePathologyIn.h"
#include "openslide-features.h"
#include "openslide.h"
#include <math.h>

namespace ALPHA
{
	namespace ROSE
	{
		struct patch {
			std::pair<int, int> coord;
			cv::Mat array;
		};

		class IOThread
		{
		public:
			explicit IOThread() {}
			explicit IOThread(IFPipelinePathologyIn* input, int start_row, int start_col, int cut_width, int cut_height);
			~IOThread();
			patch get_patch();
			cv::Mat readPatch(int x_start, int y_start, int width, int height, int level);
			void start();
			void wait();
			int get_patchlist_size();

			static std::mutex m_mutex;
			static std::list<patch> m_patchList;

			std::string m_svs_path;
			std::string m_slide_name;
			int m_start_row;
			int m_start_col;
			int m_cut_width;
			int m_cut_height;
			int m_num_images = 0;
			int m_num_blur = 0;
			int m_num_blank = 0;
			int m_num_broken = 0;
			int width;
			int height;
		private:
			void add_patch(patch&& p);
			void add_size(int& w, int& h);
			void run();
			std::thread* m_run_thread = nullptr;
			openslide_t* m_slide;
		};
	}
}

#endif 
