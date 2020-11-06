#include "PipelinePathologyUtilsFactory.h"
#include "PipelinePathologyUtils.h"
namespace ALPHA
{
	namespace Pathology 
	{
		namespace PTUtils
		{
			PipelinePathologyUtilsFactory::PipelinePathologyUtilsFactory() :
				COMM::PipelineFactoryBase()
			{

			}

			PipelinePathologyUtilsFactory::~PipelinePathologyUtilsFactory()
			{

			}

			std::shared_ptr<COMM::PipelineBase> PipelinePathologyUtilsFactory::createPipeline()
			{
				return std::make_shared<PipelinePathologyUtils>();
			}
		}
	}
}