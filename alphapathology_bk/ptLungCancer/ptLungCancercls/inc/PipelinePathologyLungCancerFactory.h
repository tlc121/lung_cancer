#if !defined(_PIPELINEPATHOLOGYLungCancerFACTORY_H)
#define _PIPELINEPATHOLOGYLungCancerFACTORY_H

#include "PipelineFactoryBase.h"
#include "PipelinePathologyLungCancerFactory_export.h"

namespace ALPHA
{
	namespace LungCancer
	{
		class PipelinePathologyLungCancerFactory : public COMM::PipelineFactoryBase {
		public:
			explicit PipelinePathologyLungCancerFactory();
			~PipelinePathologyLungCancerFactory() override;
			std::shared_ptr<COMM::PipelineBase> createPipeline() override;
			
		};
	}
}

#endif  //_PIPELINEPATHOLOGYLungCancerFACTORY_H