#include "timer.h"
#include "log.h"
#include "thread.h"
#include "util.h"
#include <memory>

namespace dzy {
Timer::Timer(uint64_t ms , std::function<void()> cb,TimerManager* manager, bool recricle):m_ms(ms),m_cb(cb)
                                                                ,m_manager(manager),m_recricle(recricle){
            m_next = GetTimeMs() + m_ms;
}
Timer::Timer(uint64_t ms){
    m_next = ms;
}

bool Timer::cancle(){
    Timer::ptr me = shared_from_this();
    //删除的自己就在第一个Timer
    RWMutex::WRLockGuard lock(m_manager->m_mutex);
    if(me == (*m_manager->m_timers.begin())){
        m_manager->m_timers.erase(me);
        m_manager->timerInsertAtFront();
        lock.unlock();
        return true;
    }else{
        auto i = m_manager->m_timers.find(me);
        if(i != m_manager->m_timers.end()){
            m_manager->m_timers.erase(i);
            lock.unlock();
            return true;
        }else{
            lock.unlock();
            return false;
        }
    }
}

bool Timer::TimerCompare::operator()(const Timer::ptr& l,const Timer::ptr& r){
    if(!l || !r){
        return true;
    }
    if(l->m_next < r->m_next){
        return true;
    }else{
        return false;
    }
}

uint64_t TimerManager::getNextTime(){
    if(m_timers.empty()){
        return ~0;
    }
    int64_t t = (int64_t)((*m_timers.begin())->m_next - GetTimeMs());
    if(t < 0){
        return 0;
    }
    return (uint64_t)t;
}


TimerManager::TimerManager(){
}
TimerManager::~TimerManager(){
}

Timer::ptr TimerManager::addTimer(uint64_t ms,std::function<void()> cb,bool recricle ){
    Timer::ptr timer(new Timer(ms,cb,this,recricle));
    RWMutex::WRLockGuard lock(m_mutex);
    auto i = m_timers.insert(timer).first;
    if(i == m_timers.begin()){
        timerInsertAtFront();
    }
    return timer;
}

Timer::ptr TimerManager::addTimerCondition(uint64_t ms,std::function<void()> cb,std::weak_ptr<void> condition,bool recricle){
    Timer::ptr timer(new Timer(ms,[cb,condition](){
            std::shared_ptr<void> shared_condition = condition.lock();
            if(shared_condition){
            cb();
            }
    },this,recricle));
    RWMutex::WRLockGuard lock(m_mutex);
    auto i = m_timers.insert(timer).first;
    if(i == m_timers.begin()){
        timerInsertAtFront();
    }
    return timer;
}
void TimerManager::addTimer(Timer::ptr timer){
    RWMutex::WRLockGuard lock(m_mutex);
    auto i = m_timers.insert(timer).first;
    if(i == m_timers.begin()){
        timerInsertAtFront();
    }
}
bool TimerManager::hasTimer(){
    return !m_timers.empty();
}
void TimerManager::getExpireCbs(std::vector<std::function<void()> >& cbs){
    RWMutex::WRLockGuard lock(m_mutex);
    Timer::ptr timer(new Timer(GetTimeMs()));
    std::vector<Timer::ptr> expireds;
    for(auto i = m_timers.begin(); i != m_timers.end(); ){
        if((*i)->m_next < GetTimeMs()){
            expireds.push_back(*i);
            m_timers.erase(i++);
        }else{
            break;
        }

    }
    
    for(auto it = expireds.begin(); it != expireds.end(); it++){
        cbs.push_back((*it)->m_cb);
        if((*it)->m_recricle){
            (*it)->m_next = GetTimeMs() + (*it)->m_ms;
            m_timers.insert(*it);
        }
    }
}

}
