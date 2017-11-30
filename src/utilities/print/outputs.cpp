#include "outputs.h"
#include "ctime"
//#include <boost/date_time/posix_time/posix_time.hpp>

enum Logger::LoggerLevel Logger::_coutLevel = Logger::InfoLevel;

bool Logger::_logTime = false;

Logger::LogWriter<Logger::DebugLevel> Logger::Debug;

Logger::LogWriter<Logger::InfoLevel> Logger::Info;

Logger::LogWriter<Logger::WarningLevel> Logger::Warn;

Logger::LogWriter<Logger::ErrorLevel> Logger::Error;

Logger::LogWriter<Logger::FatalLevel> Logger::Fatal;

Logger::LogWriter<Logger::NoLevel, true> Logger::Progress;

const std::string Logger::hline
    = "--------------------------------------------------------------------------------\n";

const std::string Logger::sharpLine
    = "################################################################################\n";



void Logger::setVerbosity(VerbosityLevel verbosityLevel)
{
    switch (verbosityLevel)
    {
        case QuietVerbosity: _coutLevel   = NoLevel; break;
        case NormalVerbosity: _coutLevel  = InfoLevel; break;
        case VerboseVerbosity: _coutLevel = DebugLevel; break;
    }
}

void Logger::outputTime(bool toStdErr)
{
    // boost::posix_time::ptime t(boost::posix_time::microsec_clock::local_time());
    // std::string str = boost::posix_time::to_simple_string(t);
    std::time_t result = std::time(nullptr);
    std::string str{std::asctime(std::localtime(&result))};
    str.pop_back();
    if (toStdErr)
        std::cerr << str << " : ";
    else
        std::cout << str << " : ";
}
