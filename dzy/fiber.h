#pragma once

#include <memory>
#include <functional>
#include <sys/types.h>
#include <sys/ucontext.h>
#include <ucontext.h>
#include "config.h"


namespace dzy {
static ConfigVar<uint32_t>::ptr g_fiber_stacksize = Config::Lookup("dzy.stack_size",(uint32_t)1024 * 1024,"fiber size");
class Fiber:public std::enable_shared_from_this<Fiber> {
public:
    typedef std::shared_ptr<Fiber> ptr;
    enum State {
        INIT,
        HOLD,
        EXCEP,
        EXEC,
        TEAM,
        READY
    };
    Fiber(std::function<void()> cb,uint32_t stacksize = 0,bool use_caller = false);
    ~Fiber();
    
    State getState();
    void setState(State state){m_state = state;}
    void swapIn();
    void swapOut();
    void reset(std::function<void()> cb);
    void callIn();
    void callOut();
    uint64_t getId();
    uint64_t getid(){return m_id;}

   private:
    Fiber();
public:
    static void YieldToHold();
    static void SetThis(Fiber* f);
    static void YieldToReady();
    static Fiber::ptr GetThis();
    static uint64_t GetTotal();
    static void MainFunc();
    static void MainFuncFiber();
private:
    ucontext_t m_ctx;
    std::function<void()> m_cb;
    uint64_t m_id = 0;
    State m_state = INIT;
    uint32_t m_stacksize =0;
    void* m_stack = nullptr;
    bool m_use_caller;

};


}
