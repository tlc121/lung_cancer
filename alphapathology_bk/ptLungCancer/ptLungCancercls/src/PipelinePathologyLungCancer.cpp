//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : LungCancer
//  @ File Name : PipelinePathologyLungCancer.cpp
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//


#include "PipelinePathologyLungCancer.h"
#include "IFPipelinePathologyLungCancerOut.h"
#include "PipelineElementPathologyLungCancerCls.h"
#include "CommonUtils.h"
#include "IFPipelinePathologyLungCancerIn.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"

//class tensorflow_operator
//{
//public:
//	tensorflow::Session* session_cls;
//};

namespace ALPHA
{
	namespace LungCancer
	{
		PipelinePathologyLungCancer::PipelinePathologyLungCancer(std::string config_path) :COMM::PipelineAlgorithmExecutor(),
			ptrPipelineIn(nullptr), ptrPipelineOut(nullptr)
		{
			this->m_config_path = config_path;
			//this->m_operator = new tensorflow_operator;
		}

		PipelinePathologyLungCancer::~PipelinePathologyLungCancer()
		{
			/*if (m_operator != NULL)
				delete m_operator;*/

		}

		void PipelinePathologyLungCancer::setUpPipeline()
		{
			//std::cout << "PipelineHemorrhagicStroke::setUpPipeline()" << std::endl;
			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "PipelinePathologyLungCancer::setUpPipeline()!");

			// step 1: classification
			std::shared_ptr<LungCancer::PipelineElementPathologyLungCancerCls>
				ptrPipelineElementPathologyLungCancerCls(new PipelineElementPathologyLungCancerCls());
			//ptrPipelineElementPathologyLungCancerCls->m_session_cls = m_operator->session_cls;
			//ptrPipelineElementPathologyLungCancerCls->m_session_qc = m_operator->session_qc;
			//ptrPipelineElementPathologyLungCancerCls->m_session_det = m_operator->session_det;
			listElement.push_back(ptrPipelineElementPathologyLungCancerCls);

		}

		//void PipelinePathologyLungCancer::read_qc_model() {
		//	libconfig::Config mConfig;
		//	ALPHA::COMM::loadCfg(this->m_config_path, mConfig);
		//	std::string graph_path_qc = mConfig.lookup("LungCancer_CLS_MIL.MODEL_QC_PATH"); //model path
		//	float gpu_ratio = mConfig.lookup("LungCancer_CLS_MIL.QC_GPU_RATIO");
		//	tensorflow::GraphDef graph_def_qc;
		//	if (!ReadBinaryProto(tensorflow::Env::Default(), graph_path_qc, &graph_def_qc).ok())
		//	{
		//		LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "load qc model FAILED");
		//	}
		//	else
		//	{
		//		LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "load qc model successfully");
		//	}

		//	tensorflow::SessionOptions sess_opt;
		//	sess_opt.config.mutable_gpu_options()->set_allow_growth(true);
		//	sess_opt.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(gpu_ratio);
		//	m_operator->session_qc = tensorflow::NewSession(sess_opt);
		//	std::cout << "Create graph" << std::endl;
		//	if (!m_operator->session_qc->Create(graph_def_qc).ok()) {
		//		std::cout << "Create graph failed" << std::endl;
		//	}
		//}

		//void PipelinePathologyLungCancer::read_cls_model() {
		//	libconfig::Config mConfig;
		//	ALPHA::COMM::loadCfg(this->m_config_path, mConfig);
		//	std::string graph_path_cls = mConfig.lookup("LungCancer_CLS_MIL.MODEL_CLS_PATH"); //model path
		//	float gpu_ratio = mConfig.lookup("LungCancer_CLS_MIL.CLS_GPU_RATIO");
		//	tensorflow::GraphDef graph_def_cls;
		//	if (!ReadBinaryProto(tensorflow::Env::Default(), graph_path_cls, &graph_def_cls).ok())
		//	{
		//		LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "load cls model FAILED");
		//	}
		//	else
		//	{
		//		LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "load cls model successfully");
		//	}

		//	tensorflow::SessionOptions sess_opt;
		//	sess_opt.config.mutable_gpu_options()->set_allow_growth(true);
		//	sess_opt.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(gpu_ratio);
		//	m_operator->session_cls = tensorflow::NewSession(sess_opt);
		//	std::cout << "Create graph" << std::endl;
		//	if (!m_operator->session_cls ->Create(graph_def_cls).ok()) {
		//		std::cout << "Create graph failed" << std::endl;
		//	}
		//}

		//void PipelinePathologyLungCancer::read_det_model() {
		//	libconfig::Config mConfig;
		//	ALPHA::COMM::loadCfg(this->m_config_path, mConfig);
		//	std::string graph_path_det = mConfig.lookup("LungCancer_CLS_MIL.MODEL_DET_PATH"); //model path
		//	float gpu_ratio = mConfig.lookup("LungCancer_CLS_MIL.DET_GPU_RATIO");
		//	tensorflow::GraphDef graph_def_det;
		//	if (!ReadBinaryProto(tensorflow::Env::Default(), graph_path_det, &graph_def_det).ok())
		//	{
		//		LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "load det model FAILED");
		//	}
		//	else
		//	{
		//		LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "load det model successfully");
		//	}

		//	tensorflow::SessionOptions sess_opt;
		//	sess_opt.config.mutable_gpu_options()->set_allow_growth(true);
		//	sess_opt.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(gpu_ratio);
		//	m_operator->session_det = tensorflow::NewSession(sess_opt);
		//	std::cout << "Create graph" << std::endl;
		//	if (!m_operator->session_det->Create(graph_def_det).ok()) {
		//		std::cout << "Create graph failed" << std::endl;
		//	}
		//}

		void PipelinePathologyLungCancer::executePipeline()
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
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "PipelinePathologyLungCancer need at least one algorithmElement! Quite pipeline computation!");
				return;
			}
			if (ptrPipelineIn == nullptr)
			{
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "nullptr input in PipelinePathologyLungCancer! Quite pipeline computation!");
				return;
			}
			(*itPre)->setInput(ptrPipelineIn);
			for (; itCurr != listElement.end(); ++itCurr, ++itPre)
			{
				(*itPre)->update();
				(*itCurr)->setInput((*itPre)->getOutput());
			}
			(*itPre)->update();
			this->ptrPipelineOut = std::dynamic_pointer_cast<IFPipelinePathologyLungCancerOut>((*itPre)->getOutput());
		}

		void PipelinePathologyLungCancer::setInput(std::shared_ptr<COMM::DataObj> Input)
		{
			ptrPipelineIn = std::dynamic_pointer_cast<LungCancer::IFPipelinePathologyLungCancerIn>(Input);
		}

		std::shared_ptr<COMM::DataObj> PipelinePathologyLungCancer::getOutput()
		{
			//TO DO: need to edit for the output

			return ptrPipelineOut;
		}
	}
}