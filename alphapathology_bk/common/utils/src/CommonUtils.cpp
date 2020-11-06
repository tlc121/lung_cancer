#include "CommonUtils.h"

namespace ALPHA {

	namespace COMM
	{

		// defind the MyLogger
		std::shared_ptr<MyLogger> MyLogger::m_logger = nullptr;

		MyLogger::MyLogger()
		{}

		MyLogger::~MyLogger()
		{
			std::cout << "delete MyLogger" << std::endl;
		}

		std::shared_ptr<MyLogger> MyLogger::getInstance()
		{
			if (m_logger == nullptr)
			{
				std::cout << "construct MyLogger" << std::endl;
				log4cplus::initialize();
				std::string  logger_config_path = AlPHA_CONFIG_ROOT_PATH;
				#ifndef WIN32
				    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(logger_config_path + "logger.cfg"));
				#else
				    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(logger_config_path + "logger_windows.cfg"));
				#endif
				m_logger.reset(new MyLogger);

				m_logger->m_rootLog = log4cplus::Logger::getRoot();
			}

			return m_logger;
		}

		void loadCfg(std::string& strConfPath, libconfig::Config& mConfig)
		{
			try
			{
				LOG4CPLUS_INFO(MyLogger::getInstance()->m_rootLog, "Config file load from " << strConfPath);
				mConfig.readFile(strConfPath.c_str());
			}
			catch (libconfig::FileIOException& e)
			{
				LOG4CPLUS_ERROR(MyLogger::getInstance()->m_rootLog, "read file [ " << strConfPath << " ] FileIOException");
				AlphaExceptionCreater(9900, CommonExceptionList[9900]);
			}
			catch (libconfig::ParseException& e)
			{
				LOG4CPLUS_ERROR(MyLogger::getInstance()->m_rootLog, "read file [ " << strConfPath << " ],ParaseException: " << e.getError() << ",line:" << e.getLine());
				AlphaExceptionCreater(9900, CommonExceptionList[9900]);
			}
			catch (...)
			{
				LOG4CPLUS_ERROR(MyLogger::getInstance()->m_rootLog, "read file [" << strConfPath << " ] unknown exception");
				AlphaExceptionCreater(9900, CommonExceptionList[9900]);
			}
		}


		// exception objection
		ALPHAExceptionObject::ALPHAExceptionObject(uint32_t returnCode, const std::string& file, unsigned int lineNumber, const std::string& desc) :
			mReturnCode(returnCode), mFile(file), mLineNumber(lineNumber), mDesc(desc)
		{
			std::ostringstream loc;
			loc << mFile << ":" << mLineNumber << ":\n" << mDesc;
			mReturnMessage = loc.str();
		}
		ALPHAExceptionObject::~ALPHAExceptionObject()
		{}
		uint32_t ALPHAExceptionObject::getReturnCode()
		{
			return mReturnCode;
		}

		std::string ALPHAExceptionObject::getReturnMessage()
		{
			return mReturnMessage;
		}

		bool IsEqual(double d1, double d2, int _absDelta)
		{
			bool original_answer = (d1 == d2);
			bool return_answer;

			if (static_cast<int>(d1) != static_cast<int>(d2))
			{
				return_answer = false;
			}
			else {
				int i1, i2;
				d1 = d1 - static_cast<int>(d1);
				d2 = d2 - static_cast<int>(d2);
				i1 = (d1 > 0) ? ((int&)d1) : ((int&)d1 - 0x80000000);
				i2 = (d2 > 0) ? ((int&)d2) : ((int&)d2 - 0x80000000);
				return_answer = (((abs(i1 - i2)) < _absDelta) ? true : false);
			}
			return return_answer;

		}
		bool IsGreaterEqual(double d1, double d2, int _greater_absDelta)
		{
			bool original_answer = (d1 >= d2);
			bool return_answer;

			return_answer = std::isgreaterequal(d1, d2);
			return return_answer;
		}
		bool IsSmallerEqual(double d1, double d2, int _greater_absDelta)
		{
			bool original_answer = (d1 <= d2);
			bool return_answer;
			return_answer = std::islessequal(d1, d2);
			//std::cout << "Original answer and"
			return return_answer;
		}





	}
}



