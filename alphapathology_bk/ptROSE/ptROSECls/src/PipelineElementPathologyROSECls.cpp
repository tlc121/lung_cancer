//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : ROSE
//  @ File Name : PipelineElementPathologyROSECls.cpp
//  @ Date : 2019/8/15
//  @ Author : weiping liu
//
//

#include "PipelineElementPathologyROSECls.h"
#include "InferLocalPathologyROSE.h"

namespace ALPHA
{
	namespace ROSE
	{
		PipelineElementPathologyROSECls::PipelineElementPathologyROSECls():
			COMM::PipelineElementAlgorithmMachineLearningBase()
		{

		}

		PipelineElementPathologyROSECls::~PipelineElementPathologyROSECls()
		{

		}

		void PipelineElementPathologyROSECls::setInput(std::shared_ptr< COMM::DataObj > Input)
		{
			ptrInterfaceData = std::dynamic_pointer_cast<IFPipelinePathologyIn>(Input);
		}

		std::shared_ptr<COMM::DataObj> PipelineElementPathologyROSECls::getOutput() {
			std::shared_ptr<COMM::DataObj> p(rose_out);
			return p;
		}

		void PipelineElementPathologyROSECls::preproc() {
			return;
		}

		void PipelineElementPathologyROSECls::infer() {
			InferLocalPathologyROSE rose_infer;
			std::string model_name = "";
			rose_out = rose_infer.predict(ptrInterfaceData.get(), model_name);
		}

		void PipelineElementPathologyROSECls::postproc() {
			return;
		}
	}
}


