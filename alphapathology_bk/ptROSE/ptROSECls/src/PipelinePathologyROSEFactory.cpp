#include "PipelinePathologyROSEFactory.h"
#include "PipelinePathologyROSE.h"
namespace ALPHA
{
	namespace ROSE
	{
		PipelinePathologyROSEFactory::PipelinePathologyROSEFactory():
			COMM::PipelineFactoryBase()
		{

		}

		PipelinePathologyROSEFactory::~PipelinePathologyROSEFactory()
		{

		}

		std::shared_ptr<COMM::PipelineBase> PipelinePathologyROSEFactory::createPipeline()
		{
			return std::make_shared<PipelinePathologyROSE>();
		}
	}
}