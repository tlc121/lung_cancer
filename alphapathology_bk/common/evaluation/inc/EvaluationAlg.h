#include "EvaluationAlg_export.h"
#include <vector>
#include <string>

#define MY_LOG_FILE_PATH "logger.cfg"

namespace ALPHA {
	namespace COMM
	{
		class EVALUATIONALG_EXPORT Evaluation {
		public:
			Evaluation(const std::string& filename);
			Evaluation(const std::string& predit, const std::string& target);
			Evaluation(std::vector<std::pair<float, int>> input);
			~Evaluation();

			float calcAccuracy();
			float calcSensitivity();
			float calcSpecficity();
			float calcCohenKappaScore();
			float calcAreaUnderCurve();
			float calc95ErrorRate();
			bool invalid_input = false;

			void writeToFile(std::string);

		private:
			std::vector<float> TP; //roc true positives
			std::vector<float> FP; //roc false positive
			std::vector<std::pair<float, int>> data; // (probablity, label)
			std::vector<std::pair<int, int>> data2; // (predit_label, trarget_label)
			float epsilon = 0.0001;

			void calcROCFromData();
		};
	};
};

