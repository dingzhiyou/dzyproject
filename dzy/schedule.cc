#include <functional>
#include <string>
#include "fiber.h"
#include "log.h"
#include "util.h"
#include "macro.h"
#include "schedule.h"
#include "hook.h"

namespace dzy {
//static thread_local Fiber::ptr t_schedule_fiber = nullptr;
static thread_local Schedule* t_schedule = nullptr;

Schedule::Schedule(int num,bool use_caller , std::string name):m_numThread(num),m_name(name){
    if(t_schedule){
        DZY_ASSERT2(false,"already being schedule");
    }
    //如果使用主线程（创建schedule的线程）
    if(use_caller){
        Fiber::GetThis();
        m_rootFiber.reset(new Fiber(std::bind(&Schedule::run_use,this),g_fiber_stacksize->getVal(),true));
        m_rootThread = Gettid();
        t_schedule = this;
        t_schedule_fiber = m_rootFiber.get()->shared_from_this();
        m_numThread--;
        m_thrsId.push_back(m_rootThread);
    }
    else{
        m_rootFiber = nullptr;
    }
}
    
Schedule::~Schedule(){

}
Schedule* Schedule::GetThis(){
        return t_schedule;
}
Fiber::ptr Schedule::GetMainFiber(){
    return t_schedule_fiber->shared_from_this();
}

void Schedule::start(){
    if(!m_stopping){
        return;
    }
    Fiber::GetThis();
    for(int i = 0;i <m_numThread ; i++){
        m_thrs.push_back(Thread::ptr(new Thread(std::bind(&Schedule::run,this))));
        m_thrsId.push_back(m_thrs[i]->getTid());
    }

}
void Schedule::tickle() {
    std::cout<< "tickle"<<std::endl;

}

void Schedule::idle(){
    while(true){
        Fiber::YieldToReady();
    }
}

bool Schedule::stopping(){
    return false;
}
void Schedule::stop(){
    m_stopping = true;
    if(m_rootFiber && m_numThread == 0
      && m_rootThread != -1 && (Fiber::GetThis()->getState() == Fiber::State::TEAM || Fiber::GetThis()->getState() == Fiber::State::INIT)){
        if(stopping()){
                return;
        }
        if(m_numThread == -1){
            DZY_ASSERT(m_rootThread != Gettid());
        }else{
            DZY_ASSERT(m_rootThread == Gettid());
        }
    }

    for(int i = 0; i < m_numThread;i++){
        tickle();
    }
    if(m_rootFiber){
        tickle();
    }
    if(m_rootFiber){
        while(!stopping()){
            m_rootFiber->callIn();
        }
    }
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) <<"safety out";
    for(int i = 0; i < m_numThread;i++){
            m_thrs[i]->join();
    }
}


void Schedule::run(){
    Fiber::GetThis();
    setHook(true);
    t_schedule_fiber = Fiber::GetThis();
    t_schedule = this;
    Fiber::ptr fiber;
    Fiber::ptr cb_fiber;
    FiberAndThread ft;
    Fiber::ptr idle_fiber(new Fiber(std::bind(&Schedule::idle,this),g_fiber_stacksize->getVal()));
    while(true){
        bool ticke_me = false;
        {
            MutexType::LockGuard lock(m_mutex);
            auto i = m_fibers.begin();
            while(i != m_fibers.end()){
                ticke_me = false;
                //指定了特定线程运行且不失本线程的话
                if(i->thread != -1 && i->thread != Gettid()){
                    i++;
                    ticke_me = true || ticke_me;
                }else{
                    break;
                }
            }
            if(ticke_me){
                tickle();
            }
            if(i != m_fibers.end()){
                    ft.reset();
                    ft = (*i);
                   m_fibers.erase(i);
            }
        }
        if(ft.fiber && ft.fiber->getState() != Fiber::TEAM){
            ft.fiber->swapIn();
            if(ft.fiber->getState() == Fiber::READY){
                schedule(ft.fiber);
            }
            else if(ft.fiber->getState() != Fiber::EXCEP && ft.fiber->getState() != Fiber::EXEC && ft.fiber->getState() != Fiber::TEAM){
                ft.fiber->setState(Fiber::HOLD);
            }
            ft.reset();
        }else if(ft.cb){
            cb_fiber.reset(new Fiber(ft.cb));
            cb_fiber->swapIn();
            if(cb_fiber->getState() == Fiber::READY){
                schedule(cb_fiber);
            }else if(cb_fiber->getState() != Fiber::EXCEP && cb_fiber->getState() != Fiber::EXEC && cb_fiber->getState() != Fiber::TEAM){
                cb_fiber->setState(Fiber::HOLD);
            }
            ft.reset();
        }else{
                if(idle_fiber->getState() == Fiber::TEAM || idle_fiber->getState() == Fiber::EXCEP){
                    break;
                }
                idle_fiber->swapIn();
        }

    }
}

void Schedule::run_use(){
    Fiber::GetThis();
    setHook(true);
    Fiber::ptr fiber;
    Fiber::ptr cb_fiber;
    FiberAndThread ft;
    Fiber::ptr idle_fiber(new Fiber(std::bind(&Schedule::idle,this),g_fiber_stacksize->getVal(),true));
    while(true){
        bool ticke_me = false;
        {
            MutexType::LockGuard lock(m_mutex);
            auto i = m_fibers.begin();
            while(i != m_fibers.end()){
                ticke_me = false;
                //指定了特定线程运行且不失本线程的话
                if(i->thread != -1 && i->thread != Gettid()){
                    i++;
                    ticke_me = true || ticke_me;
                }else{
                    break;
                }
            }
            if(ticke_me){
                tickle();
            }
            if(i != m_fibers.end()){
                    ft.reset();
                    ft = (*i);
                    m_fibers.erase(i);
            }
            lock.unlock();
        }

        if(ft.fiber && ft.fiber->getState() != Fiber::TEAM){
            ft.fiber->swapIn();
            if(ft.fiber->getState() == Fiber::READY){
                schedule(ft.fiber);
            }
            else if(ft.fiber->getState() != Fiber::EXCEP && ft.fiber->getState() != Fiber::EXEC && ft.fiber->getState() != Fiber::TEAM){
                ft.fiber->setState(Fiber::HOLD);
            }
            ft.reset();
        }else if(ft.cb){
            cb_fiber.reset(new Fiber(ft.cb));
            cb_fiber->swapIn();
            if(cb_fiber->getState() == Fiber::READY){
                schedule(cb_fiber);
            }else if(cb_fiber->getState() != Fiber::EXCEP && cb_fiber->getState() != Fiber::EXEC && cb_fiber->getState() != Fiber::TEAM){
                cb_fiber->setState(Fiber::HOLD);
            }
            ft.reset();
        }else{
                if(idle_fiber->getState() == Fiber::TEAM || idle_fiber->getState() == Fiber::EXCEP){
                    std::cout<<"idle exit"<<std::endl;
                    break;
                }
                m_idleFiberCount++;
                idle_fiber->swapIn();
                m_idleFiberCount--;
                ft.reset();
        }

    }
    DZY_ASSERT(false);
    Fiber::GetThis()->callOut();
}


}
