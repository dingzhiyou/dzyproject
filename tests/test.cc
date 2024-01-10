#include "../dzy/log.h"
#include "../dzy/util.h"

int main(int argc,char** argv){
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
    auto log = dzy::LoggerMgr::GetInstance()->GetRoot();
   DZY_LOG_FATAL(log)<<"dadadw3ruhgfrieygh"<<std::endl;
    
   return  0;

}
