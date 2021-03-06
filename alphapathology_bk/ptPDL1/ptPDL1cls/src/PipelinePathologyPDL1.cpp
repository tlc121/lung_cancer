//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : PDL1
//  @ File Name : PipelinePathologyPDL1.cpp
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//


#include "PipelinePathologyPDL1.h"
#include "IFPipelinePathologyPDL1Out.h"
#include "PipelineElementPathologyPDL1Cls.h"
#include "CommonUtils.h"
#include "IFPipelinePathologyPDL1In.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"

class tensorflow_operator
{
public:
	tensorflow::Session* session_seg;
};

namespace ALPHA
{
	namespace PDL1
	{
		PipelinePathologyPDL1::PipelinePathologyPDL1(std::string config_path) :COMM::PipelineAlgorithmExecutor(),
			ptrPipelineIn(nullptr), ptrPipelineOut(nullptr)
		{
			this->m_config_path = config_path;
			this->m_operator = new tensorflow_operator;
		}

		PipelinePathologyPDL1::~PipelinePathologyPDL1()
		{
			if (m_operator != NULL)
				delete m_operator;

		}

		void PipelinePathologyPDL1::setUpPipeline()
		{
			//std::cout << "PipelineHemorrhagicStroke::setUpPipeline()" << std::endl;
			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "PipelinePathologyPDL1::setUpPipeline()!");

			// step 1: classification
			std::shared_ptr<PDL1::PipelineElementPathologyPDL1Cls>
				ptrPipelineElementPathologyPDL1Cls(new PipelineElementPathologyPDL1Cls());
			ptrPipelineElementPathologyPDL1Cls->m_session_seg= m_operator->session_seg;
			listElement.push_back(ptrPipelineElementPathologyPDL1Cls);

		}

		void PipelinePathologyPDL1::read_seg_model() {
			libconfig::Config mConfig;
			ALPHA::COMM::loadCfg(this->m_config_path, mConfig);
			std::string graph_path_seg = mConfig.lookup("PT_PDL1.MODEL_SEG_PATH"); //model path
			float gpu_ratio = mConfig.lookup("PT_PDL1.SEG_GPU_RATIO");
			tensorflow::GraphDef graph_def_seg;
			if (!ReadBinaryProto(tensorflow::Env::Default(), graph_path_seg, &graph_def_seg).ok())
			{
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "load seg model FAILED");
			}
			else
			{
				LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "load seg model successfully");
			}

			tensorflow::SessionOptions sess_opt;
			sess_opt.config.mutable_gpu_options()->set_allow_growth(true);
			sess_opt.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(gpu_ratio);
			m_operator->session_seg = tensorflow::NewSession(sess_opt);
			std::cout << "Create graph" << std::endl;
			if (!m_operator->session_seg->Create(graph_def_seg).ok()) {
				std::cout << "Create graph failed" << std::endl;
			}
		}


		void PipelinePathologyPDL1::executePipeline()
		{
			//std::cout << "PipelineHemorrhagicStroke::executePipeline()" << std::endl;
			//std::list<std::shared_ptr<PipelineElementBase>> listElement;
			std::list<std::shared_ptr<COMM::PipelineElementBase>>::iterator
				itPre = listElement.begin();
			auto itCurr = itPre;
			if (itCurr != listElement.end())
			{
				++itCurr;
			}
			else
			{
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "PipelinePathologyPDL1 need at least one algorithmElement! Quite pipeline computation!");
				return;
			}
			if (ptrPipelineIn == nullptr)
			{
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "nullptr input in PipelinePathologyPDL1! Quite pipeline computation!");
				return;
			}
			(*itPre)->setInput(ptrPipelineIn);
			for (; itCurr != listElement.end(); ++itCurr, ++itPre)
			{
				(*itPre)->update();
				(*itCurr)->setInput((*itPre)->getOutput());
			}
			(*itPre)->update();
			this->ptrPipelineOut = std::dynamic_pointer_cast<IFPipelinePathologyPDL1Out>((*itPre)->getOutput());
		}

		void PipelinePathologyPDL1::setInput(std::shared_ptr<COMM::DataObj> Input)
		{
			ptrPipelineIn = std::dynamic_pointer_cast<PDL1::IFPipelinePathologyPDL1In>(Input);
		}

		std::shared_ptr<COMM::DataObj> PipelinePathologyPDL1::getOutput()
		{
			//TO DO: need to edit for the output

			return ptrPipelineOut;
		}
	}
}