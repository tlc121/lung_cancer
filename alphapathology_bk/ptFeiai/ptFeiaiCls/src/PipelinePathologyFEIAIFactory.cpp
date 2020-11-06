#include "PipelinePathologyFEIAIFactory.h"
#include "PipelinePathologyFEIAI.h"
namespace ALPHA
{
	namespace FEIAI
	{
		PipelinePathologyFEIAIFactory::PipelinePathologyFEIAIFactory() :
			COMM::PipelineFactoryBase()
		{

		}

		PipelinePathologyFEIAIFactory::~PipelinePathologyFEIAIFactory()
		{

		}

		std::shared_ptr<COMM::PipelineBase> PipelinePathologyFEIAIFactory::createPipeline()
		{
			return std::make_shared<PipelinePathologyFEIAI>("");
		}
	}
}