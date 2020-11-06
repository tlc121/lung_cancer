#include "PipelinePathologyPDL1Factory.h"
#include "PipelinePathologyPDL1.h"
namespace ALPHA
{
	namespace PDL1
	{
		PipelinePathologyPDL1Factory::PipelinePathologyPDL1Factory() :
			COMM::PipelineFactoryBase()
		{

		}

		PipelinePathologyPDL1Factory::~PipelinePathologyPDL1Factory()
		{

		}

		std::shared_ptr<COMM::PipelineBase> PipelinePathologyPDL1Factory::createPipeline()
		{
			return std::make_shared<PipelinePathologyPDL1>("");
		}
	}
}