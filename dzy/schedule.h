#pragma once

#include <memory>
#include <functional>
#include "thread.h"
#include "fiber.h"
#include <vector>
#include <list>
namespace dzy {

class Schedule {
public:
    typedef std::shared_ptr<Schedule> ptr;
    typedef Mutex MutexType;
    Schedule(int num,bool use_caller = true, std::string name = "Schedule_Name");
    
    virtual ~Schedule();
    std::string getName(){return m_name;}

    static Schedule::ptr GetThis();
    static Fiber::ptr GetMainFiber();

    void start();
    void stop();

    template<class FiberOrCallback>
    void schedule(FiberOrCallback f,int thr){
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
                need_tickle = schedule(&(*i),1) || need_tickle;
        }
        if(need_tickle){
            tickle();
        }
    }

    template<class FiberOrCallback>
    bool scheduleUnLock(FiberOrCallback f,int thr){
            bool flag = m_fibers.empty();
            FiberAndThread fc(f,thr);
            if(fc.fiber || fc.cb){
                m_fibers.push_back(fc);
            }
            return flag;
    }
protected:
    void tickle();
private:
    struct FiberAndThread {
        Fiber::ptr fiber;
        std::function<void()> cb;
        int thread;

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
    };
private:
    std::vector<Thread::ptr> m_thrs;
    std::list<FiberAndThread> m_fibers;
    MutexType m_mutex;
    std::string m_name;


};

}
