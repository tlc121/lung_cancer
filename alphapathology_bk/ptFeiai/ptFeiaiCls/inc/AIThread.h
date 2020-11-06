#ifndef _AI_THREAD_H_
#define _AI_THREAD_H_


#include <thread>
#include <mutex>
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
#include "IFPipelinePathologyFEIAIOUT.h"
#include "IFPipelinePathologyFEIAIIn.h"
#include <stdint.h>
#include "openslide-features.h"
#include "openslide.h"
#include "IoThread.h"

namespace ALPHA
{
	namespace FEIAI {

		class AIThread
		{
		public:
			explicit AIThread() {}
			explicit AIThread(std::string config_path, IFPipelinePathologyFEIAIOut* output, IOThread* io): m_configPath(config_path), m_output(output), m_io(io) {}

			~AIThread() {}
			void startThread();
			IFPipelinePathologyFEIAIOut* m_output;

		private:
			std::string m_configPath;
			IOThread* m_io;
			void run();
		};
	}
}
#endif