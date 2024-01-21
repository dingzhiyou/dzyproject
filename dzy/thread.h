#pragma once

#include <iterator>
#include <memory>
#include <functional>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <iostream>


namespace dzy {

class Semphore {
public:
    Semphore(const unsigned int count = 0);
    ~Semphore();
   int wait();
    void notify();

private:
    Semphore(Semphore&) = delete ;
    Semphore(Semphore&&) = delete;
    Semphore& operator=(Semphore&) = delete;
private:
    sem_t m_semaphore;   

};

template<class T>
struct ScopedLockImpl {
public:
    ScopedLockImpl(T& mutex):m_mutex(mutex){
        mutex.lock();
        isLocked = true;
    }
    ~ScopedLockImpl(){
        unlock();
    }
    void lock(){
        if(!isLocked){
            m_mutex.lock();
        }
        isLocked = true;
    }
    void unlock(){
        if(isLocked){
            m_mutex.unlock();
        }
        isLocked = false;
    }
private:
    T& m_mutex;
    bool isLocked;
};


template<class T>
struct WRScopedLockImpl {
public:
    WRScopedLockImpl(T& mutex):m_mutex(mutex){
        mutex.wrlock();
        isLocked = true;
    }
    ~WRScopedLockImpl(){
        unlock();
    }
    void lock(){
        if(!isLocked){
            m_mutex.wrlock();
        }
        isLocked = true;
    }
    void unlock(){
        if(isLocked){
            m_mutex.unlock();
        }
        isLocked = false;
    }
private:
    T& m_mutex;
    bool isLocked;
};


template<class T>
struct RDScopedLockImpl {
public:
    RDScopedLockImpl(T& mutex):m_mutex(mutex){
        mutex.rdlock();
        isLocked = true;
    }
    ~RDScopedLockImpl(){
        unlock();
    }
    void lock(){
        if(!isLocked){
            m_mutex.rdlock();
        }
        isLocked = true;
    }
    void unlock(){
        if(isLocked){
            m_mutex.unlock();
        }
        isLocked = false;
    }
private:
    T& m_mutex;
    bool isLocked;
};

class Mutex {
public:
    typedef ScopedLockImpl<Mutex> LockGuard;
    Mutex();
    ~Mutex();
    int lock();
    void unlock();
private:
    pthread_mutex_t m_mutex;
};

class RWMutex {
public:
    typedef RDScopedLockImpl<RWMutex> RDLockGuard;
    typedef WRScopedLockImpl<RWMutex> WRLockGuard;
    RWMutex();
    ~RWMutex();
    void rdlock();
    void wrlock();
    void unlock();
private:
    pthread_rwlock_t m_mutex;
};


class SpinMutex {
public:
    typedef ScopedLockImpl<SpinMutex> LockGuard;
    SpinMutex();
    ~SpinMutex();
    int lock();
    void unlock();
private:
    pthread_spinlock_t m_mutex;
};



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
        Thread& operator=(Thread&) = delete ;
        static void* run(void* arg);

private:
        pid_t m_tid = -1;
        pthread_t m_thread = 0;
        std::function<void()> m_cb;
        std::string m_name;
        Semphore m_sem;
};


}
