//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : ROSE
//  @ File Name : PipelinePathologyROSE.cpp
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//


#include "PipelinePathologyROSE.h"
#include "PipelineElementPathologyROSECls.h"
#include "IFPipelinePathologyIn.h"
#include "IFPipelinePathologyROSEOut.h"
#include "CommonUtils.h"


namespace ALPHA
{
	namespace ROSE
	{
		PipelinePathologyROSE::PipelinePathologyROSE(): COMM::PipelineAlgorithmExecutor(),
			ptrPipelineIn(nullptr), ptrPipelineOut(nullptr)
		{

		}
		PipelinePathologyROSE::~PipelinePathologyROSE()
		{

		}

		void PipelinePathologyROSE::setUpPipeline()
		{
			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "PipelinePathologyROSE::setUpPipeline()!");

			// step 1: classification
			std::shared_ptr<PipelineElementPathologyROSECls>
				ptrPipelineElementPathologyROSECls(new PipelineElementPathologyROSECls());
			listElement.push_back(ptrPipelineElementPathologyROSECls);
			
		}

		void PipelinePathologyROSE::executePipeline()
		{
			std::list<std::shared_ptr<COMM::PipelineElementBase>>::iterator
				itPre = listElement.begin();
			auto itCurr = itPre;
			if (itCurr != listElement.end())
			{
				++itCurr;
			}
			else
			{
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "PipelinePathologyROSE need at least one algorithmElement! Quite pipeline computation!");
				return;
			}
			if (ptrPipelineIn == nullptr)
			{
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "nullptr input in PipelinePathologyROSE! Quite pipeline computation!");
				return;
			}
			(*itPre)->setInput(ptrPipelineIn);
			for (; itCurr != listElement.end(); ++itCurr, ++itPre)
			{
				(*itPre)->update();
				(*itCurr)->setInput((*itPre)->getOutput());
			}
			(*itPre)->update();
			ptrPipelineOut = std::dynamic_pointer_cast<IFPipelinePathologyROSEOut>((*itPre)->getOutput());
		}

		void PipelinePathologyROSE::setInput(std::shared_ptr<COMM::DataObj> Input)
		{
			ptrPipelineIn = std::dynamic_pointer_cast<ALPHA::ROSE::IFPipelinePathologyIn>(Input);
			LOG4CPLUS_INFO(COMM::MyLogger::getInstance()->m_rootLog, "read path: " << ptrPipelineIn->m_svs_path);
		}

		std::shared_ptr<COMM::DataObj> PipelinePathologyROSE::getOutput()
		{	

			return ptrPipelineOut;
		}
	}
}