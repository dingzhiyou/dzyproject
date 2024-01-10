#ifndef DZY_LOG_H__
#define DZY_LOG_H__
#include <iostream>
#include <fstream>
#include <ostream>
#include <map>
#include <sstream>
#include <memory>
#include <string>
#include <stdint.h>
#include <vector>
#include <chrono>
#include <ctime>
#include "util.h"
#include "singleton.h"

#define DZY_LOG_LEVEL(logger,level) \
    if(logger->getLevel() <= level) \
      dzy::LogEventWarp(dzy::LogEvent::ptr(new dzy::LogEvent(__FILE__,__LINE__,dzy::Gettid(),dzy::GetFiberId(),time(0),logger,level))).getSS()

#define DZY_LOG_DEBUG(logger) DZY_LOG_LEVEL(logger,dzy::LogLevel::DEBUG)
#define DZY_LOG_INFO(logger) DZY_LOG_LEVEL(logger,dzy::LogLevel::INFO)
#define DZY_LOG_WARN(logger) DZY_LOG_LEVEL(logger,dzy::LogLevel::WARN)
#define DZY_LOG_ERROR(logger) DZY_LOG_LEVEL(logger,dzy::LogLevel::ERROR)
#define DZY_LOG_FATAL(logger) DZY_LOG_LEVEL(logger,dzy::LogLevel::FATAL)

#define DZY_LOG_ROOT()   dzy::Singleton<dzy::LogManager>::GetInstance()->GetRoot()

namespace dzy{

class Logger;

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

class LogEvent {
public:
    LogEvent(const char* file,int32_t line,int32_t thread,uint32_t fiber,time_t time,std::shared_ptr<Logger> logger,LogLevel::Level level);
    typedef std::shared_ptr<LogEvent> ptr;
    ~LogEvent();
    LogEvent();
    const char* getFile() const {return m_file;}
    int32_t getLine()const {return m_line ;}
    int32_t getThreadId()const {return m_threadId ;}
    uint32_t getFiberId()const {return  m_fiberId ;}
    std::string getContent()const {return m_ss.str();}
    time_t getTime()const{return m_time;}
    std::stringstream&  getSS() {return m_ss;};
    std::shared_ptr<Logger> getLogger(){return m_logger;}
    LogLevel::Level getLevel(){return m_level;}

private:
    const char* m_file = nullptr;
    int32_t m_line = 0;
    int32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    std::stringstream m_ss;
    time_t m_time;
    std::shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;

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
            FormatterItem(const std::string& fmt = ""){}
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
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override {
                os << event->getContent();
           }
};
class NewLineFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<NewLineFormatItem> ptr;
           NewLineFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
                os <<std::endl;
           }
};
class StringFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<StringFormatItem> ptr;
           StringFormatItem(const std::string& str=""):m_str{str}{}
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
                os <<m_str;
           }

        private:
           std::string m_str;
};



class TableFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<TableFormatItem> ptr;
           TableFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override {
                os <<"    ";
           }
};
class LevelFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<LevelFormatItem> ptr;
           LevelFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
                os << LogLevel::ToString(level);
           }
};

class FileFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<FileFormatItem> ptr;
           FileFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
                os << event->getFile();
           }
};
class LineFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<LineFormatItem> ptr;
           LineFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
                os << event->getLine();
           }
};
class FiberFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<FiberFormatItem> ptr;
           FiberFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
                os << event->getFiberId();
           }
};

class ThreadIdFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<ThreadIdFormatItem> ptr;
           ThreadIdFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
                os << event->getThreadId();
           }
};
class TimeFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<TimeFormatItem> ptr;
           TimeFormatItem(const std::string& format = "%Y:%m:%d %H:%M:%S"):m_format(format){}
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
                auto now = std::chrono::system_clock::now();
                std::time_t current_time = std::chrono::system_clock::to_time_t(now);
                char buff[80];
                std::strftime(buff,sizeof(buff),"%Y-%m-%d  %H:%M:%S",std::localtime(&current_time));
                os << buff;
           }
        private:
           std::string m_format;
};


class LoggerAppender {
   public:
        typedef std::shared_ptr<LoggerAppender> ptr;
        virtual ~LoggerAppender() {};
        virtual  void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
        LoggerFormater::ptr  getFormatter(){return m_formatter;};
        void setFormatter(LoggerFormater::ptr formatter){m_formatter = formatter;}
   protected:
        LogLevel::Level m_level;
        LoggerFormater::ptr m_formatter;
};


class Logger :public  std::enable_shared_from_this<Logger>{
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
    LogLevel::Level getLevel(){return m_level;}
private:
    std::vector<LoggerAppender::ptr> m_appenders;
    std::string m_name;
    LogLevel::Level m_level;
    LoggerFormater::ptr m_formatter;
};



class LogEventWarp{
public:
    LogEventWarp(LogEvent::ptr event):m_event(event){ 
    }
    ~LogEventWarp(){
        std::shared_ptr<Logger> it = m_event->getLogger();
            it->log(m_event->getLevel(),m_event);
    }
    std::stringstream& getSS(){return m_event->getSS();}
private:
    LogEvent::ptr m_event;
};

class StdoutAppender : public LoggerAppender {
public:
    typedef std::shared_ptr<StdoutAppender> ptr;
    StdoutAppender(LogLevel::Level level = LogLevel::Level::DEBUG,LoggerFormater::ptr formatter = nullptr);
    void log(std::shared_ptr<Logger>logger ,LogLevel::Level level,LogEvent::ptr event) override;
private:
};

class FileAppender: public LoggerAppender {
public:
    typedef std::shared_ptr<FileAppender> ptr;
    FileAppender(LogLevel::Level level = LogLevel::Level::DEBUG,const std::string& name = "log");
    void log(std::shared_ptr<Logger>logger ,LogLevel::Level level,LogEvent::ptr event) override;
    void reopen();
private:
    std::string m_name;
    std::ofstream m_filestream;
};


class LogManager {

public:
    LogManager();
    ~LogManager(){}
    Logger::ptr GetLogger(const std::string& name);
    Logger::ptr GetRoot();
    std::map<std::string ,Logger::ptr>& GetLoggers(){
        return m_loggers;
    }
private:
    std::map<std::string ,Logger::ptr> m_loggers;
};
typedef Singleton<LogManager> LoggerMgr;

}
#endif
