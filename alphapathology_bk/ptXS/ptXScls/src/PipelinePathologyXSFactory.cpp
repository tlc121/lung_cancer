#include "PipelinePathologyXSFactory.h"
#include "PipelinePathologyXS.h"
namespace ALPHA
{
	namespace XS
	{
		PipelinePathologyXSFactory::PipelinePathologyXSFactory() :
			COMM::PipelineFactoryBase()
		{

		}

		PipelinePathologyXSFactory::~PipelinePathologyXSFactory()
		{

		}

		std::shared_ptr<COMM::PipelineBase> PipelinePathologyXSFactory::createPipeline()
		{
			return std::make_shared<PipelinePathologyXS>("");
		}
	}
}