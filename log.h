#pragma once

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdint.h>
#include <vector>



namespace dzy{

class Logger;

class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();
    const char* getFile() const {return m_file;}
    int32_t getLine()const {return m_line ;}
    int32_t getThreadId()const {return m_threadId ;}
    uint32_t getFiberId()const {return  m_fiberId ;}
    std::string getContent()const {return m_content;}
    time_t getTime()const{return m_time;}

private:
    const char* m_file = nullptr;
    int32_t m_line = 0;
    int32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    std::string m_content;
    time_t m_time;
};

class LogLevel{
public:
    enum Level{
            UNKNOW = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
    static const char* ToString(LogLevel::Level level);
};


class LoggerFormater {
public:
    typedef std::shared_ptr<LoggerFormater> ptr;


    LoggerFormater(const std::string& parttern);
    std::string formatter(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event);
    void init();

    class FormatterItem {
        public:
            typedef std::shared_ptr<FormatterItem>   ptr;
            FormatterItem(const std::string& fmt = "") ;
            virtual ~FormatterItem(){}
            virtual void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
    };

private:
    std::vector<FormatterItem::ptr> m_items;
    std::string m_pattern;
};

class MessageFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<MessageFormatItem> ptr;
           MessageFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os << event->getContent();
           }
};
class NewLineFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<NewLineFormatItem> ptr;
           NewLineFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os <<std::endl;
           }
};
class StringFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<StringFormatItem> ptr;
           StringFormatItem(const std::string& str=""):m_str{str}{}
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os <<m_str;
           }

        private:
           std::string m_str;
};



class TableFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<TableFormatItem> ptr;
           TableFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os <<"    ";
           }
};
class LevelFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<LevelFormatItem> ptr;
           LevelFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os << LogLevel::ToString(level);
           }
};

class FileFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<FileFormatItem> ptr;
           FileFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os << event->getFile();
           }
};
class LineFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<LineFormatItem> ptr;
           LineFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os << event->getLine();
           }
};
class FiberFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<FiberFormatItem> ptr;
           FiberFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os << event->getFiberId();
           }
};

class ThreadIdFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<ThreadIdFormatItem> ptr;
           ThreadIdFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os << event->getThreadId();
           }
};
class TimeFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<TimeFormatItem> ptr;
           TimeFormatItem(const std::string& format = "%Y:%m:%d %H:%M:%S"):m_format(format){}
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os << event->getTime();
           }

        private:
           std::string m_format;
};






class LoggerAppender {
   public:
        typedef std::shared_ptr<LoggerAppender> ptr;
        virtual ~LoggerAppender() {};
        virtual  void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event);
   protected:
        LogLevel::Level m_level;
        LoggerFormater::ptr m_formatter;
};


class Logger :std::enable_shared_from_this<Logger>{
public:
    typedef std::shared_ptr<Logger> ptr;
    
    Logger(const std::string& name = "ROOT");
    void log(LogLevel::Level  level,LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    void setLevel(LogLevel::Level level);
    void addAppener(LoggerAppender::ptr appender);
    void delAppener(LoggerAppender::ptr appender);
    std::string getName()const {return m_name;}
private:
    std::vector<LoggerAppender::ptr> m_appenders;
    std::string m_name;
    LogLevel::Level m_level;
};

class StdoutAppender : public LoggerAppender {
public:
    typedef std::shared_ptr<StdoutAppender> ptr;
    StdoutAppender(LogLevel::Level level,LoggerFormater::ptr formatter);
    void log(std::shared_ptr<Logger>logger ,LogLevel::Level level,LogEvent::ptr event) override;
private:
};

class FileAppender: public LoggerAppender {
public:
    typedef std::shared_ptr<FileAppender> ptr;
    FileAppender(LogLevel::Level level,LoggerFormater::ptr formatter,const std::string& name);
    void log(std::shared_ptr<Logger>logger ,LogLevel::Level level,LogEvent::ptr event) override;
    void reopen();
private:
    std::string m_name;
    std::ofstream m_filestream;
};



}

