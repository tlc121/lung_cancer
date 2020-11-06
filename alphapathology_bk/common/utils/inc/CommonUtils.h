#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>  
#include <sstream>  
#include <string>
#include <errno.h>
  
#include <windows.h>  
#include <direct.h>  
#define AlPHA_CONFIG_ROOT_PATH "D:\\config\\"


#include <log4cplus/logger.h>
#include <log4cplus/configurator.h> 
#include <log4cplus/layout.h> 
#include <log4cplus/loggingmacros.h> 
#include <log4cplus/helpers/stringhelper.h> 

#include "libconfig.h++"
#include "CommonExceptionList.h"

#include "CommonUtils_export.h"
#include <cmath>



#define AlphaExceptionCreater(code,discription)                                                    \
    {                                                                                              \
    std::ostringstream message;                                                                    \
    message << "ALPHA_ERROR: "<<discription;                                                       \
    uint32_t returnCode = code;                                                                    \
    std::string file_name = __FILE__;                                                              \
    unsigned int line_num = __LINE__;                                                              \
    ALPHA::COMM::ALPHAExceptionObject e_(returnCode, file_name, line_num, message.str());          \
    throw e_; /* Explicit naming to work around Intel compiler bug.  */                            \
    }


namespace ALPHA {

	namespace COMM
	{

		// alpha exception
		class COMMONUTILS_EXPORT ALPHAExceptionObject
		{
		public:
			ALPHAExceptionObject(uint32_t returnCode, const std::string& file, unsigned int lineNumber = 0, const std::string & desc = "None");
			~ALPHAExceptionObject();
			uint32_t getReturnCode();
			std::string getReturnMessage();
		private:
			uint32_t mReturnCode;
			std::string mFile;
			unsigned int mLineNumber;
			std::string mDesc;
			std::string mReturnMessage;
		};

		// Get Logger
		class COMMONUTILS_EXPORT MyLogger
		{
		public:
			static std::shared_ptr<MyLogger> getInstance();
			log4cplus::Logger m_rootLog;
			~MyLogger();

		private:
			MyLogger();
			static std::shared_ptr<MyLogger> m_logger;
		};

		// get config
		void COMMONUTILS_EXPORT loadCfg(std::string& strConfPath, libconfig::Config& mConfig);

		// string to number
		template <class Type>
		Type stringToNum(const std::string& str)
		{
			std::istringstream iss(str);
			Type num;
			iss >> num;
			return num;
		}

		// load nii
		template <typename TImage>
		void loadNii(const std::string& path, TImage* image)
		{
			LOG4CPLUS_INFO(MyLogger::getInstance()->m_rootLog, "start running loadNii for path: " << path);
			typedef itk::ImageSeriesReader<TImage> ReaderType;
			typename ReaderType::Pointer reader = ReaderType::New();
			reader->SetImageIO(itk::NiftiImageIO::New());
			reader->SetFileName(path.c_str());

			try
			{
				reader->Update();
			}
			catch (itk::ExceptionObject& err)
			{
				LOG4CPLUS_ERROR(MyLogger::getInstance()->m_rootLog, "Load Err: " << err.what());
				//std::map<uint32_t, std::string>CommExceptionList;
				//CommExceptionList[1001] = "no file.";
				//AlphaExceptionCreater(1001, CommExceptionList[1001]);
				AlphaExceptionCreater(1001, err.GetDescription());
			}
			image->Graft(reader->GetOutput());
		}

		// save nii
		template <typename TImage>
		void saveNii(const std::string& path, TImage* const image)
		{
			LOG4CPLUS_INFO(MyLogger::getInstance()->m_rootLog, "start running saveNii in path: " << path);
			typedef  itk::ImageFileWriter<TImage> WriterType;
			typename WriterType::Pointer writer = WriterType::New();
			writer->SetFileName(path);
			writer->SetImageIO(itk::NiftiImageIO::New());
			writer->SetInput(image);
			try
			{
				writer->Update();
			}
			catch (itk::ExceptionObject& err)
			{
				LOG4CPLUS_ERROR(MyLogger::getInstance()->m_rootLog, "SaveNii Err: " << err.what());
				AlphaExceptionCreater(1002, err.GetDescription());
			}
		}

		// file exists check
		inline bool fileExistsCheck(const std::string& name) {
			std::ifstream f(name.c_str());
			return f.good();
		}


		/*
		* @param[in] path
		* @return int 0:不存在，1:为文件夹, -1:为文件
		*/
		inline int isFileOrDir(std::string path)
		{
   
			DWORD dwAttr = GetFileAttributes(path.c_str());
			//Not exist     
			if (dwAttr == 0xFFFFFFFF) {
				return 0;
			}
			//exist     
			else if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
				return 1;
			}
			//is File     
			else {
				return -1;
			}

		}


		bool COMMONUTILS_EXPORT IsEqual(double d1, double d2, int _absDelta);
		bool COMMONUTILS_EXPORT IsGreaterEqual(double d1, double d2, int _greater_absDelta);
		bool COMMONUTILS_EXPORT IsSmallerEqual(double d1, double d2, int _smaller_absDelta);


	};

};

