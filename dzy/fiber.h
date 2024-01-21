#pragma once

#include <memory>
#include <functional>
#include <sys/ucontext.h>
#include <ucontext.h>


namespace dzy {

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
    Fiber(std::function<void()> cb,uint32_t stacksize = 0);
    ~Fiber();

    void swapIn();
    void swapOut();
    void reset(std::function<void()> cb);
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
private:
    ucontext_t m_ctx;
    std::function<void()> m_cb;
    uint64_t m_id = 0;
    State m_state = INIT;
    uint32_t m_stacksize =0;
    void* m_stack = nullptr;

};


}
