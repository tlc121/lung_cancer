#include "PipelinePathologyLungCancerFactory.h"
#include "PipelinePathologyLungCancer.h"
namespace ALPHA
{
	namespace LungCancer
	{
		PipelinePathologyLungCancerFactory::PipelinePathologyLungCancerFactory() :
			COMM::PipelineFactoryBase()
		{

		}

		PipelinePathologyLungCancerFactory::~PipelinePathologyLungCancerFactory()
		{

		}

		std::shared_ptr<COMM::PipelineBase> PipelinePathologyLungCancerFactory::createPipeline()
		{
			return std::make_shared<PipelinePathologyLungCancer>("");
		}
	}
}