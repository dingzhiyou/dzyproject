#pragma once

#include <memory>
#include <functional>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


namespace dzy {
class Thread {
public:
         typedef std::shared_ptr<Thread> ptr;
         Thread(std::function<void()> cb,const std::string name = "UNKNOW_THREAD");
        ~Thread();
        void join();
        const std::string& getName()const {return m_name;}
        const pid_t& getTid ()const {return m_tid;}

        static Thread* GetThis();
        static void SetThis();
        static const std::string& GetName();
       

private:
        Thread(Thread&) = delete ;
        Thread(Thread&&) = delete ;
        Thread& operator()(Thread&) = delete ;
        static void* run(void* arg);

private:
        pid_t m_tid = -1;
        pthread_t m_thread = 0;
        std::function<void()> m_cb;
        std::string m_name;
};


}
