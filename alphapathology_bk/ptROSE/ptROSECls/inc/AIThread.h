#if !defined(_AITHREAD_H)
#define _AITHREAD_H

#include <thread>
#include "InferLocalBase.h"
#include "IFPipelinePathologyROSEOut.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "IOThread.h"

namespace ALPHA
{
	namespace ROSE
	{
		class AIThread
		{
		public:
			explicit AIThread() {}
			explicit AIThread(std::string config_path, IOThread* io, IFPipelinePathologyROSEOut* output, bool* io_complete_flag) : 
				m_configPath(config_path), m_io(io), m_output(output), m_io_complete_flag(io_complete_flag){}

			~AIThread() {}
			void start();
			void wait();

		private:
			std::string m_configPath;
			IOThread* m_io;
			IFPipelinePathologyROSEOut* m_output;
			bool* m_io_complete_flag;
			void run();
			std::thread* m_run_thread = nullptr;
		};
	}
}

#endif