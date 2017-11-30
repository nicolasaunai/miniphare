#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <iostream>
#include <string>


#include <iostream>
#include <sstream>

// largely inspired from
// https://github.com/juandesant/WSClean/blob/ede3f3a419f36ffe93612f797a989247052e8e49/wsclean/wsclean/logger.h
class Logger
{
public:
    enum LoggerLevel {
        NoLevel      = 5,
        FatalLevel   = 4,
        ErrorLevel   = 3,
        WarningLevel = 2,
        InfoLevel    = 1,
        DebugLevel   = 0
    };

    enum VerbosityLevel { QuietVerbosity, NormalVerbosity, VerboseVerbosity };

    static const std::string hline;
    static const std::string sharpLine;


    template<enum LoggerLevel Level, bool ToStdErr = false>
    class LogWriter
    {
    public:
        LogWriter()
            : _atNewLine(true)
        {
        }

        LogWriter &operator<<(const std::string &str)
        {
            //					boost::mutex::scoped_lock lock(_mutex);
            size_t start = 0, end;
            while (std::string::npos != (end = str.find('\n', start)))
            {
                outputLinePart(str.substr(start, end - start + 1), true);
                start = end + 1;
            }
            outputLinePart(str.substr(start, str.size() - start), false);
            return *this;
        }
        LogWriter &operator<<(const char *str)
        {
            (*this) << std::string(str);
            return *this;
        }
        LogWriter &operator<<(const char c)
        {
            //					boost::mutex::scoped_lock lock(_mutex);
            outputLinePart(std::string(1, c), c == '\n');
            return *this;
        }
        template<typename S>
        LogWriter &operator<<(const S &str)
        {
            std::ostringstream stream;
            stream << str;
            (*this) << stream.str();
            return *this;
        }
        void flush()
        {
            //					boost::mutex::scoped_lock lock(_mutex);
            if (ToStdErr)
                std::cerr.flush();
            else
                std::cout.flush();
        }

    private:
        //				boost::mutex _mutex;
        bool _atNewLine;

        void outputLinePart(const std::string &str, bool endsWithCR)
        {
            if ((int)_coutLevel <= (int)Level && !str.empty())
            {
                if (_atNewLine && _logTime)
                    outputTime(ToStdErr);
                if (ToStdErr)
                    std::cerr << str;
                else
                    std::cout << str;
                _atNewLine = endsWithCR;
            }
        }
    };

    static void setVerbosity(VerbosityLevel verbosityLevel);
    static bool isVerbose() { return _coutLevel == DebugLevel; }
    static void SetLogTime(bool logTime) { _logTime = logTime; }
    static bool logTime() { return _logTime; }

    static class LogWriter<DebugLevel> Debug;
    static class LogWriter<InfoLevel> Info;
    static class LogWriter<WarningLevel> Warn;
    static class LogWriter<ErrorLevel> Error;
    static class LogWriter<FatalLevel> Fatal;
    static class LogWriter<NoLevel, true> Progress;

private:
    Logger() {}

    static void outputTime(bool toStdErr);

    static enum LoggerLevel _coutLevel;

    static bool _logTime;
};

#endif // OUTPUTS_H
