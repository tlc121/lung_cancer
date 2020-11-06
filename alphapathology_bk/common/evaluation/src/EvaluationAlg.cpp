#include "EvaluationAlg.h"
#include "EvaluationExceptionList.h"
#include "CommonUtils.h"

#include <iostream>
//#include <memory>
//#include <fstream>  
//#include <sstream>  
//#include <errno.h>
//#include <algorithm>
//#include <limits>
//#include <iomanip>
//#include <math.h>

#ifdef WIN32  
//#include <windows.h>  
//#include <direct.h>  
#else  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <dirent.h>  
#endif 

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h> 
#include <log4cplus/layout.h> 
#include <log4cplus/loggingmacros.h> 
#include <log4cplus/helpers/stringhelper.h> 

#include "libconfig.h++"


namespace ALPHA {
	namespace COMM
	{
		Evaluation::Evaluation(const std::string& filename) {
			float score;
			int label;
			std::ifstream inFile(filename.c_str());

			if (!inFile) {
				LOG4CPLUS_ERROR(COMM::MyLogger::getInstance()->m_rootLog, "file not found");
				AlphaExceptionCreater(5001, EvaluationExceptionList[5001]);
			}

			// read the data from the file
			while (!inFile.eof()) {
				inFile >> score >> label;
				if (!inFile) break;
				this->data.push_back(std::make_pair(score, label));
			}
		}

		Evaluation::Evaluation(const std::string& predit, const std::string& target) {
			if (predit.size() != target.size())
				this->invalid_input = true;

			std::string p, t;
			int pp, tt;
			for (auto ps = predit.begin(), ts = target.begin(); (ps != predit.end()) && (ts != target.end()); ++ps, ++ts) {
				p = *ps;
				t = *ts;
				pp = std::stoi(p);
				tt = std::stoi(t);
				data2.push_back(std::make_pair(pp, tt));

				if (pp > 1 || tt > 1)
					this->invalid_input = true;
			}

		}

		Evaluation::Evaluation(std::vector<std::pair<float, int>> input) : data(input) {}
		Evaluation::~Evaluation() {}

		void Evaluation::writeToFile(std::string filename) {
			std::ofstream outfile(filename.c_str());
			for (int i = 0; i < this->TP.size(); i++) {
				outfile << FP[i] << "\t" << TP[i] << std::endl;
			}
			std::cout << "Evaluation curve points saved to file " << filename << std::endl;
		}

		float Evaluation::calc95ErrorRate() {
			// TODO - Implement this
			return 1.0;
		}

		float Evaluation::calcAreaUnderCurve() {
			calcROCFromData();
			// TODO - Implement this
			int size = TP.size();
			float q1, q2, p1, p2;
			q1 = FP[0];
			q2 = TP[0];
			float area = 0.0;
			for (int i = 1; i < size; ++i) {
				p1 = FP[i];
				p2 = TP[i];
				area += sqrt(pow(((1 - q1) + (1 - p1)) / 2 * (q2 - p2), 2));
				q1 = p1;
				q2 = p2;
			}
			return area;
		}

		// Main Evaluation algorithm from the paper
		// Evaluation Graphs: Notes and Practical Considerations for
		// Researchers - Tom Fawcett (Algorithm 2)
		// @TECHREPORT{Fawcett04Evaluationgraphs:,
		//     author = {Tom Fawcett},
		//     title = {Evaluation Graphs: Notes and Practical Considerations for Researchers},
		//     year = {2004}
		// }
		void Evaluation::calcROCFromData() {
			//sort the data by classification score
			//std::sort(data.begin(), data.end(), compare);
			std::sort(data.begin(), data.end(),
				[](const std::pair<float, int>& i, const std::pair<float, int>& j)
			{
				return i.first > j.first;
			});

			int L = data.size();
			int P = 0;
			int N = 0;

			// count positive and negative class occurences
			for (int j = 0; j < data.size(); j++) {
				if (data[j].second == 1)
				{
					P++;
				}
				else
				{
					N++;
				}
			}
			double f_i;
			bool label;
			//init FP TP counters
			double FP = 0;
			double TP = 0;
			double f_prev = -std::numeric_limits<double>::infinity();
			std::vector<std::pair<float, float> > R;
			//loop through all data
			for (int i = 0; i < L; i++) {
				f_i = data[i].first;
				label = data[i].second;
				if (f_i != f_prev)
				{
					// add points to Evaluation curves
					this->TP.push_back(TP / P);
					this->FP.push_back(FP / N);

					f_prev = f_i;
				}
				if (label == 1)
				{
					TP = TP + 1;
				}
				else
				{
					FP = FP + 1;
				}
			}
			//add point 1-1
			this->TP.push_back(TP / P);
			this->FP.push_back(FP / N);
		}


		float Evaluation::calcAccuracy() {
			int correct = 0;
			for (auto& pair : this->data2)
			{
				correct += pair.first ^ pair.second ^ 1;
			}
			return correct / static_cast<float>(this->data2.size());
		}

		float Evaluation::calcSensitivity() {
			int tp = 0;
			int allPos = 0;
			for (auto& pair : this->data2)
			{
				tp += pair.first * pair.second;
				allPos += pair.second;
			}
			return tp / static_cast<float>(allPos + this->epsilon);
		}

		float Evaluation::calcSpecficity() {
			int tn = 0, allNeg = 0;
			for (auto& pair : this->data2)
			{
				tn += (pair.first ^ 1) * (pair.second ^ 1);
				allNeg += (pair.second ^ 1);
			}
			return tn / static_cast<float>(allNeg + this->epsilon);
		}


		// Cohen Kappa Score: kappa = (Po - Pe) / (1-Pe)
		// The kappa statistic, which is a number between - 1 and 1. 
		// The maximum value means complete agreement; zero or lower means chance agreement. 
		// see also: https://www.statisticshowto.datasciencecentral.com/cohens-kappa-statistic/
		float Evaluation::calcCohenKappaScore() {
			float Po, Pe, kappa;
			long length = static_cast<long>(data2.size());
			// Step 1: Calculate Po (the observed proportional agreement)
			Po = calcAccuracy();
			// Step 2: Find the probability that the raters would randomly both say Yes.
			long randYesPred = 0, randYesTarget = 0;
			for (auto& pair : this->data2)
			{
				randYesPred += pair.first;
				randYesTarget += pair.second;
			}

			if (randYesPred == randYesTarget)
				//same results, return 1
				return 1.0;

			float randYes = static_cast<float>(randYesPred) / length * randYesTarget / length;

			// Step 3: Calculate the probability that the raters would randomly both say No.
			float randNO = static_cast<float>(length - randYesPred) / length * (length - randYesTarget) / length;

			// Step 4: Calculate Pe. Add your answers from Step 2 and 3 to get the overall probability that the raters would randomly agree.
			Pe = randYes + randNO;
			//Step 5: Insert your calculations into the formula and solve
			kappa = (Po - Pe) / (1 - Pe + this->epsilon);
			return kappa;
		}
	}
}



