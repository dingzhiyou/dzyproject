#include "log.h"
#include "util.h"
#include <iterator>
#include <memory>
#include <sstream>
#include <map>
#include <functional>
 
namespace  dzy{

LogEvent::LogEvent(const char* file,int32_t line,int32_t thread,uint32_t fiber,time_t time,std::shared_ptr<Logger> logger,LogLevel::Level level):m_file(file),m_line(line),m_threadId(thread)
           ,m_fiberId(fiber),m_time(time),m_logger(logger),m_level(level){
}
LogEvent:: ~LogEvent(){
    


}
const char* LogLevel::ToString(LogLevel::Level level){
    switch (level) {
#define XX(name)  \
        case LogLevel::name:  \
            return #name ;  \
            break;   \

    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);

#undef XX
    default:
    return "UNKNOW";
  }
    return  "UNKNOW";
}

const LogLevel::Level LogLevel::ToLevel(std::string& str){
    if(str == "UNKNOW"){
        return LogLevel::UNKNOW;
    }else if(str == "DEBUG"){
        return LogLevel::DEBUG;
    }else if(str == "INFO"){
        return LogLevel::INFO;
    }else if(str == "ERROR"){
        return LogLevel::ERROR;
    }else if(str == "FATAL"){
        return LogLevel::FATAL;
    }
    else return LogLevel::UNKNOW;


}

Logger::Logger(const std::string& name):m_name(name){
    m_formatter.reset(new LoggerFormater("%d%T[%p]%T%f:%l%T%c%T%t%T%m \n"));
}
void Logger::addAppener(LoggerAppender::ptr appender){
    if(!appender->getFormatter()){
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender);
}
void Logger::log(LogLevel::Level  level,LogEvent::ptr event){
    if(level >= m_level){
        for(auto it : m_appenders){
            it->log(shared_from_this(),level, event);
        }
    }
}

void Logger::delAppener(LoggerAppender::ptr appender){
    for(auto it = m_appenders.begin();it != m_appenders.end(); it++){
        if(*it == appender){
            m_appenders.erase(it);
            break;
        }
    }
}
void Logger:: debug(LogEvent::ptr event){
    log(LogLevel::DEBUG, event);
}
void Logger:: info(LogEvent::ptr event){
    log(LogLevel::INFO, event);
}
void Logger:: warn(LogEvent::ptr event){
    log(LogLevel::WARN, event);
}
void Logger:: error(LogEvent::ptr event){
    log(LogLevel::ERROR, event);
}
void Logger:: fatal(LogEvent::ptr event){
    log(LogLevel::FATAL, event);
}
void Logger:: setLevel(LogLevel::Level level){
    m_level = level;
}
StdoutAppender::StdoutAppender(LogLevel::Level level,LoggerFormater::ptr formatter){
        m_level = level;
        m_formatter = formatter;
}
void StdoutAppender::log(std::shared_ptr<Logger>logger,LogLevel::Level level,LogEvent::ptr event){ 
    if(level >= m_level){
        std::cout << m_formatter->formatter(logger,level, event);
    }
}

void FileAppender::reopen(){
    if(m_filestream){
        m_filestream.close();
    }
    m_filestream.open(m_name);
}
FileAppender::FileAppender(LogLevel::Level level,const std::string& name):m_name(name){
        m_level = level;
        reopen();
}
void FileAppender::log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
    if(level >= m_level){
        m_filestream << m_formatter->formatter(logger,level, event);
    }
}

LoggerFormater::LoggerFormater(const std::string& parttern):m_pattern(parttern){
    init();
}
std::string LoggerFormater::formatter(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
    std::stringstream os;
    for(auto it : m_items){
        it->formatter(os,logger,level, event);
    }
    return os.str();
}




class NameFormatItem: public LoggerFormater::FormatterItem {
        public:
           typedef std::shared_ptr<MessageFormatItem> ptr;
           NameFormatItem(const std::string& str=""){};
           void formatter(std::stringstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
                os << logger->getName();
           }
};




void LoggerFormater::init()
{
	std::vector<std::tuple<std::string,std::string,int> > vec;
  	std:: string nstr;
	for(size_t i = 0; i< m_pattern.size();++i)
	{
		if(m_pattern[i] != '%')
		{
			nstr.append(1,m_pattern[i]);
			continue;
		}

		if((i + 1) < m_pattern.size())
		{
			if(m_pattern[i+1] == '%')
			{
				nstr.append(1,'%');
				continue;
			}
		}

		size_t n  = i+1;
		int fmt_status = 0;
		size_t fmt_begin = 0;

		std::string str;
		std::string fmt;
		while(n < m_pattern.size())
		{
			if(!std::isalpha(m_pattern[n]) && m_pattern[n] != '{'
					&& m_pattern[n] != '}')
			{
				break;
			}

			if(fmt_status == 0)
			{
				if(m_pattern[n] == '{')
				{
					str = m_pattern.substr(i+1,n-i-1);
					fmt_status = 1;
					fmt_begin = n;
					++n;
					continue;
				}
			}
			if(fmt_status == 1)
			{
				if(m_pattern[n] == '}')
				{
					fmt = m_pattern.substr(fmt_begin+1,n-fmt_begin - 1);
					fmt_status = 2;
					break;
				}
			}
			++n;
		}

		if(fmt_status == 0)
		{
			if(!nstr.empty())
			{
				vec.push_back(std::make_tuple(nstr,std::string(),0));
				nstr.clear();
			}
			str = m_pattern.substr(i+1,n-i-1);
			vec.push_back(std::make_tuple(str,fmt,1));
			i = n - 1;
		}else if(fmt_status == 1)
		{
			std::cout<<"pattern prase error : "<<m_pattern << " - "<<m_pattern.substr(i)<<std::endl;
    			vec.push_back(std::make_tuple("<<pattern_error>>",fmt,0));
		}
		else if(fmt_status == 2)
		{
			if(!nstr.empty())
			{
				vec.push_back(std::make_tuple(nstr,"",0));
				nstr.clear();
			}
			vec.push_back(std::make_tuple(str,fmt,1));
			i = n - 1;
		}
	}
	if(!nstr.empty())
	{
		vec.push_back(std::make_tuple(nstr,"",0));
	}

		static std::map<std::string,std::function<FormatterItem::ptr(const std::string& str)> >
			s_format_items = {
			#define XX(str,C) \
			{#str,[](const std::string& fmt){return FormatterItem::ptr(new C(fmt));}}	
				XX(m,MessageFormatItem),
				XX(p,LevelFormatItem),
				XX(c,NameFormatItem),
				XX(T,TableFormatItem),
				XX(t,ThreadIdFormatItem),
				XX(n,NewLineFormatItem),
				XX(d,TimeFormatItem),
				XX(f,FileFormatItem),
				XX(F,FiberFormatItem),
				XX(l,LineFormatItem),
			#undef  XX
		};


		for(auto& i : vec)
		{
			if(std::get<2>(i) == 0)
			{
				m_items.push_back(FormatterItem::ptr(new StringFormatItem(std::get<0>(i))));
			}
			else
			{
				auto it = s_format_items.find(std::get<0>(i));
				if(it == s_format_items.end())
				{
					m_items.push_back(FormatterItem::ptr(new StringFormatItem("<<format error>>" 
		    						+ std::get<0>(i) + ">>")));
				}
				else
				{
					m_items.push_back(it->second(std::get<1>(i)));
				}
				
			}
		}
}


LogManager::LogManager(){
        auto logger = Logger::ptr(new Logger("dzy"));
        logger->addAppener(LoggerAppender::ptr(new StdoutAppender()));
        m_loggers.insert(std::make_pair("root",logger));
}
Logger::ptr LogManager::GetLogger(const std::string& name){
    auto it = m_loggers.find(name);
    if(it != m_loggers.end()){
        return it->second;
    }
    else {
        auto root = GetLogger("root");
        return root;
    }
}

Logger::ptr LogManager::GetRoot(){
        auto root = GetLogger("root");
        return root;
}

class Init {
public:
    Init(){
        Singleton<LogManager>::GetInstance()->GetRoot();
    }
};
Init init;


}



