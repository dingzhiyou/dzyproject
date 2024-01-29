#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <stdexcept>
#include "util.h"
#include "log.h"
#include "thread.h"

namespace dzy{

static std::shared_ptr<Logger> g_logger = LoggerMgr::GetInstance()->GetLogger("system");
static thread_local Thread* t_thread = nullptr;

Thread::Thread(std::function<void()> cb,const std::string name):m_cb(cb),m_name(name){
    int rt = pthread_create(&m_thread, nullptr,&Thread::run, this);
    if(rt != 0){
        DZY_LOG_ERROR(g_logger) << "pthread_create err rt = "<<rt;
    }
    m_sem.wait();
    
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
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    thread->m_tid = Gettid();
    pthread_setname_np(pthread_self(), thread->m_name.substr(0,15).c_str());
    std::function<void()> cb;
    thread->m_sem.notify();
    cb.swap(thread->m_cb);
    cb();
    return 0;
}

Semphore::Semphore(const unsigned int count){
        if(sem_init(&m_semaphore,0,count)){
                throw std::logic_error("sem_init error");
        }

}
int Semphore::wait(){
    while(true){
        if(!sem_wait(&m_semaphore)){
            return 0;
        }
    }

}
void Semphore::notify(){
    if(sem_post(&m_semaphore)){
        throw std::logic_error("sem_post error");
    }
}

Semphore::~Semphore(){
    sem_destroy(&m_semaphore);

}

Mutex::Mutex(){
    pthread_mutex_init(&m_mutex,nullptr);
}
Mutex::~Mutex(){
    pthread_mutex_destroy(&m_mutex);
}
int Mutex::lock(){
    while(true){
        if(!pthread_mutex_lock(&m_mutex)){
            return 0;
        }
    }

}
void Mutex::unlock(){
    int ret = pthread_mutex_unlock(&m_mutex);
    if(ret == 0){
    }
}
RWMutex::RWMutex(){
    if(pthread_rwlock_init(&m_mutex,nullptr)){
        throw std::logic_error("pthread_rwlock_init() error");
    }
}
RWMutex::~RWMutex(){
    pthread_rwlock_destroy(&m_mutex);

}
void RWMutex::rdlock(){
    while(true){
        if(!pthread_rwlock_rdlock(&m_mutex)){
            return;
        }
    }
}
void RWMutex::wrlock(){
    while(true){
        if(!pthread_rwlock_wrlock(&m_mutex)){
            return;
        }
    }
}


void RWMutex::unlock(){
    while(true){
        if(!pthread_rwlock_unlock(&m_mutex)){
            std::cout<<"1"<<std::endl;
            return;
        }
    }

}
SpinMutex::SpinMutex(){
        pthread_spin_init(&m_mutex, 0);
}
SpinMutex::~SpinMutex(){
        pthread_spin_destroy(&m_mutex);
}
int SpinMutex::lock(){
    while(true){
        if(!pthread_spin_lock(&m_mutex)){
            return 0;
        }
    }           
}
void SpinMutex::unlock(){
    pthread_spin_unlock(&m_mutex);

}
}
