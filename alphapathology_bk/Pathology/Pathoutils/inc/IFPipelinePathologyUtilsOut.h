#if !defined(_IFPIPELINEPATHOLOGYUtilsOUT_H)
#define _IFPIPELINEPATHOLOGYUtilsOUT_H

#include "IFBase.h"
#include <map>
#include <string>
#include <vector>

namespace ALPHA
{
	namespace Pathology 
	{
		namespace PTUtils
		{
			class IFPipelinePathologyUtilsOut : public COMM::IFBase {
			public:
				IFPipelinePathologyUtilsOut() :COMM::IFBase() {}
				~IFPipelinePathologyUtilsOut() override {}
				bool image_broken = false;
				bool image_notclear = false;
				std::vector<unsigned int> bbox;
				std::vector<std::pair<unsigned int, unsigned int>> pos;
			};
		}
	}
}



#endif  //_IFPIPELINEPATHOLOGYUtilsOUT_H