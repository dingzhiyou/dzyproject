#pragma once

#include <bits/stdint-uintn.h>
#include <memory>
#include <functional>
#include "thread.h"
#include <set>
#include <vector>

namespace dzy {

class TimerManager;
class Timer :public std::enable_shared_from_this<Timer> {
friend class TimerManager;
public:
    typedef std::shared_ptr<Timer> ptr;

    bool cancle();
    struct TimerCompare{
        bool operator()(const Timer::ptr& l,const Timer::ptr& r);
    };
private:
    Timer(uint64_t ms , std::function<void()> cb,TimerManager* manager,bool recricle = false);
    Timer(uint64_t ms);

    
private:
    uint64_t m_ms = 0;
    std::function<void()> m_cb = nullptr;
    TimerManager* m_manager = nullptr;
    uint64_t m_next= 0;
    bool m_recricle = false;

};

class TimerManager {
friend class Timer;
public: 
    typedef RWMutex MutexType  ;
    TimerManager();
    virtual ~TimerManager();
    Timer::ptr addTimer(uint64_t ms,std::function<void()> cb,bool recricle = false);
    Timer::ptr addTimerCondition(uint64_t ms,std::function<void()> cb,std::weak_ptr<void> condition,bool recricle = false);
    void addTimer(Timer::ptr timer);
    bool hasTimer();
    void getExpireCbs(std::vector<std::function<void()> >& cbs);
    uint64_t getNextTime();
    virtual void timerInsertAtFront() = 0;
private:
    std::set<Timer::ptr,Timer::TimerCompare> m_timers;
    MutexType m_mutex;
};



}
