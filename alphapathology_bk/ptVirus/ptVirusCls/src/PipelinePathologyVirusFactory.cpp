#include "PipelinePathologyVirusFactory.h"
#include "PipelinePathologyVirus.h"
namespace ALPHA
{
	namespace VIRUS
	{
		PipelinePathologyVirusFactory::PipelinePathologyVirusFactory():
			COMM::PipelineFactoryBase()
		{

		}

		PipelinePathologyVirusFactory::~PipelinePathologyVirusFactory()
		{

		}

		std::shared_ptr<COMM::PipelineBase> PipelinePathologyVirusFactory::createPipeline()
		{
			return std::make_shared<PipelinePathologyVirus>();
		}
	}
}