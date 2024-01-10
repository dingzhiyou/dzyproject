#include "../dzy/log.h"
#include "../dzy/util.h"

int main(int argc,char** argv){
   
  auto log = dzy::LoggerMgr::GetInstance()->GetRoot();
   DZY_LOG_FATAL(log)<<"111111111111111111";
   DZY_LOG_INFO(log)<<"222222222222222";
   DZY_LOG_DEBUG(log)<<"333333333333333";
   DZY_LOG_ERROR(log)<<"444444444444444";
   DZY_LOG_ERROR(log)<<"5555555555555";
   DZY_LOG_FATAL(log)<<"66666666666666";
   

    dzy::Logger::ptr root(new dzy::Logger());
   root->addAppener(dzy::LoggerAppender::ptr(new dzy::StdoutAppender));
   std::string str("./log");
   std::string format("%p %d %T");
    

   
   root->addAppener(dzy::LoggerAppender::ptr(new dzy::FileAppender(dzy::LogLevel::DEBUG,str)));
   dzy::LogEvent::ptr event(new dzy::LogEvent(__FILE__,__LINE__,dzy::Gettid(),dzy::Gettid(),0,root,dzy::LogLevel::DEBUG));
   event->getSS()<<"hello dzy";
   root->log(dzy::LogLevel::DEBUG, event);
   DZY_LOG_DEBUG(root) << "aaaa";
   DZY_LOG_DEBUG(root) << "bbbb";
   DZY_LOG_DEBUG(root) << "aaaa"<<"dada";
   DZY_LOG_DEBUG(root) << "aaaa";
   
   return  0;

}
