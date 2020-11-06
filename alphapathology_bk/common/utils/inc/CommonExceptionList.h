#pragma once

#include <map>
#include <string>

namespace ALPHA
{
	namespace COMM
	{
		// Common excpetion list 9000-9999
		static std::map<uint32_t, std::string> CommonExceptionList{
			// 9000-9019 infer serving
			{9000, "tensorflow serving error."},                                                                    //serving error
			{9001, "the output size not 3 dimension for InferTensorFlowServerSegmentation."},                       //对于InferTensorFlowServerSegmentation输出数据不是三维的

			// 9020-9039 tensorRT serving
			{9020, "tensorRT serving error."},                                                                    //serving error

			// 9100-9199 imageProcess
			{9100, "input path is empty."},
			{9101, "no files in series folder."},
			{9102, "itk load failed."},
			{9103, "itk resample failed."},
			{9104, "invalid dcm type for CT"},

			// 9900-9999 common utils
			{9900, "config file error."},                                                                         //config file error
		};


	};
};



