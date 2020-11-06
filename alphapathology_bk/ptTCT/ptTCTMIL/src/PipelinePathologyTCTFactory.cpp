#include "PipelinePathologyTCTFactory.h"
#include "PipelinePathologyTCT.h"
namespace ALPHA
{
	namespace TCT
	{
		PipelinePathologyTCTFactory::PipelinePathologyTCTFactory() :
			COMM::PipelineFactoryBase()
		{

		}

		PipelinePathologyTCTFactory::~PipelinePathologyTCTFactory()
		{

		}

		std::shared_ptr<COMM::PipelineBase> PipelinePathologyTCTFactory::createPipeline()
		{
			return std::make_shared<PipelinePathologyTCT>("");
		}
	}
}