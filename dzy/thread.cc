#include <functional>
#include <memory>
#include <pthread.h>
#include "util.h"
#include "log.h"
#include "thread.h"

namespace dzy{

static std::shared_ptr<Logger> g_logger = LoggerMgr::GetInstance()->GetLogger("system");
static thread_local Thread* t_thread = nullptr;

Thread::Thread(std::function<void()> cb,const std::string name):m_cb(cb),m_name(name){

    std::cout<<"-aaaaaaa"<<std::endl;
    int rt = pthread_create(&m_thread, nullptr,&Thread::run, this);
    if(rt != 0){
        DZY_LOG_ERROR(g_logger) << "pthread_create err rt = "<<rt;
    }
    
}
Thread::~Thread(){
    if(m_thread){
        pthread_detach(m_thread);
    }
}
void Thread::join(){
    if(m_thread){
        pthread_join(m_thread,nullptr);
    }
    m_thread = 0;
}


Thread* Thread::GetThis(){
    return t_thread;

 }
void  Thread::SetThis(){
        

 }
const std::string& GetName(){
        return t_thread->getName();
 }

void* Thread::run(void* arg){
    std::cout<<"------------"<<std::endl;
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    thread->m_tid = Gettid();
    pthread_setname_np(pthread_self(), thread->m_name.substr(0,15).c_str());
    std::function<void()> cb;
    cb.swap(thread->m_cb);
    cb();
    return 0;
}



}
