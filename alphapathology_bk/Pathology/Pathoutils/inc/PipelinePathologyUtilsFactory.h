#if !defined(_PIPELINEPATHOLOGYUtilsFACTORY_H)
#define _PIPELINEPATHOLOGYUtilsFACTORY_H

#include "PipelineFactoryBase.h"
#include "PipelinePathologyUtilsFactory_export.h"

namespace ALPHA
{
	namespace Pathology 
	{
		namespace PTUtils
		{
			class PipelinePathologyUtilsFactory : public COMM::PipelineFactoryBase {
			public:
				explicit PipelinePathologyUtilsFactory();
				~PipelinePathologyUtilsFactory() override;
				std::shared_ptr<COMM::PipelineBase> createPipeline() override;

			};
		}
	}
}

#endif  //_PIPELINEPATHOLOGYUtilsFACTORY_H