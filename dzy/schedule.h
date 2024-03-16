#pragma once

#include <memory>
#include <map>
#include <atomic>
#include <functional>
#include "log.h"
#include "thread.h"
#include "fiber.h"
#include <sched.h>
#include <vector>
#include <list>

namespace dzy {

static thread_local Fiber::ptr t_schedule_fiber = nullptr;
class Schedule {
public:
    typedef std::shared_ptr<Schedule> ptr;
    typedef Mutex MutexType;
    Schedule(int num,bool use_caller = true, std::string name = "Schedule_Name");
    
    virtual ~Schedule();
    std::string getName(){return m_name;}

    static Schedule* GetThis();
    static Fiber::ptr GetMainFiber();

    void start();
    void stop();
    bool hasIdleFiber(){return m_idleFiberCount > 0;}

    template<class FiberOrCallback>
    void schedule(FiberOrCallback f,int thr = -1){
            MutexType::LockGuard lock(m_mutex);
            bool need_tickle = false;
            need_tickle = scheduleUnLock(f, thr);
            if(need_tickle){
                tickle();
            }
    }
    
    template<class FiberCbIterator>
    void schedule(FiberCbIterator begin ,FiberCbIterator end){
        bool need_tickle = false;
        for(auto i = begin ;
                 i != end; i++){
            //TODO
            //这里写死了
                need_tickle = schedule(&(*i),-1) || need_tickle;
        }
        if(need_tickle){
            tickle();
        }
    }

    template<class FiberOrCallback>
    bool scheduleUnLock(FiberOrCallback f,int thr = -1){
            bool flag = m_fibers.empty();
            FiberAndThread fc(f,thr);
            if(fc.fiber || fc.cb){
                m_fibers.push_back(fc);
            }
            return flag;
    }
protected:
    virtual void tickle();
    virtual void idle();

    void run();
    void run_use();
    virtual bool stopping();

private:
    struct FiberAndThread {
        Fiber::ptr fiber;
        std::function<void()> cb;
        int thread = -1;

        FiberAndThread(Fiber::ptr f,int thr):fiber(f){
            thread = thr;
        }

        FiberAndThread(std::function<void()> c,int thr):cb(c){
            thread = thr;
        }

        FiberAndThread(Fiber::ptr* f,int thr){
            fiber.swap(*f);
            thread = thr;
        }
        FiberAndThread(std::function<void()>* c,int thr){
            cb.swap(*c);
            thread = thr;
        }
        FiberAndThread():thread(-1){

        }
        void reset(){
            fiber = nullptr;
            cb = nullptr;
        }
    };
protected:
    std::atomic<uint32_t> m_idleFiberCount{0};
    std::atomic<uint32_t> m_activeFiberCount{0};
    std::vector<pid_t> m_thrsId;
    bool m_stopping = true;
    
    
private:
    int m_numThread = 0;
    std::vector<Thread::ptr> m_thrs;
    std::list<FiberAndThread> m_fibers;
    MutexType m_mutex;
    std::string m_name;
    Fiber::ptr m_rootFiber;
    pid_t m_rootThread = -1;


};

}
