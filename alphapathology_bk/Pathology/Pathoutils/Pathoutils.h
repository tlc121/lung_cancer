#include "Pathoutils_export.h"
#include <vector>
#include <string>

#define MY_LOG_FILE_PATH "logger.cfg"

namespace ALPHA {
	namespace COMM
	{	

		class PATHOUTILS_EXPORT Pathoutils {
		public: 
			Pathoutils(const std::string& img_path); //image path
			~Pathoutils();

			
			int getArea();
			std::vector<unsigned int> get_bboxes();
			std::vector<std::pair<unsigned int, unsigned int>> get_points();

		private:
			std::string img_path;
			std::vector<unsigned int> bboxes;
			bool isClear = false;
			bool isbroken = false;
			std::vector<std::pair<unsigned int, unsigned int>> points;
			
		};
	};
};
