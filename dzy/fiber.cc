#include "fiber.h"
#include <atomic>
#include <bits/stdint-uintn.h>
#include <cerrno>
#include <exception>
#include <functional>
#include <iterator>
#include <memory>
#include <ucontext.h>
#include "config.h"
#include "log.h"
#include "macro.h"
#include "util.h"
#include "schedule.h"
namespace dzy {

extern thread_local Fiber::ptr t_schedule_fiber;
static std::atomic<uint64_t> s_fiber_id{0};
static std::atomic<uint64_t> s_fiber_count{0};

static thread_local Fiber* t_fiber = nullptr; //当前正在执行的协程
static thread_local Fiber::ptr t_threadFiber = nullptr;//线程的主协程


class MallocStackSize {
public:
    static void* Malloc(uint64_t size){
        return malloc(size);
    }
    
    static void Dealloc(void* vp,size_t size){
        free(vp);
    }
};
uint64_t Fiber::getId(){
    return m_id;
}
Fiber::Fiber(std::function<void()> cb,uint32_t stacksize,bool use_caller):m_cb(cb),m_id(++s_fiber_id),m_use_caller(use_caller){

        DZY_LOG_DEBUG(DZY_LOG_ROOT()) <<"fiberId: "<<m_id;
        ++s_fiber_count;
        m_state = INIT;
        m_stacksize =  stacksize == 0 ?  g_fiber_stacksize->getVal() : stacksize;
        if(getcontext(&m_ctx)){
            DZY_ASSERT2(false,"getcontext error");
        }
        m_ctx.uc_link = nullptr;
        m_stack = MallocStackSize::Malloc(m_stacksize);
        m_ctx.uc_stack.ss_sp = m_stack;
        m_ctx.uc_stack.ss_size = m_stacksize;
    
        if(use_caller){
            makecontext(&m_ctx, &Fiber::MainFuncFiber,0);
        }else{
            makecontext(&m_ctx, &Fiber::MainFunc,0);
        }
}
Fiber::~Fiber(){
    //DZY_LOG_DEBUG(DZY_LOG_ROOT()) <<"~fiberId: "<<m_id;
    --s_fiber_count;
    if(m_stack){
        if(m_state == Fiber::HOLD){
        }
        DZY_ASSERT2(m_state == INIT || m_state == TEAM,std::to_string(m_id));
        MallocStackSize::Dealloc(m_stack, m_stacksize);
        m_stack = nullptr;
        m_stacksize = 0;
    }else {
        DZY_ASSERT(m_state == EXEC);
        DZY_ASSERT(!m_cb);
        DZY_ASSERT(t_fiber == t_threadFiber.get());
        Fiber* cur = t_fiber;
        DZY_ASSERT(cur == this);
        SetThis(nullptr);
    }
}

void Fiber::swapIn(){
    DZY_ASSERT(m_state != EXEC && m_state != EXCEP);
    SetThis(this);
    m_state = EXEC;
    if(swapcontext(&(Schedule::GetMainFiber().get()->m_ctx), &m_ctx)){
        DZY_ASSERT2(false, "swapcontext error");
    }
}
void Fiber::swapOut(){
    SetThis(Schedule::GetMainFiber().get());
    if(swapcontext(&m_ctx,&(Schedule::GetMainFiber().get()->m_ctx))){
        DZY_ASSERT2(false, "swapcontext error");
    }
}
void Fiber::reset(std::function<void()> cb){
    DZY_ASSERT2(m_state == INIT || m_state == TEAM, "reset fiber error");

    if(getcontext(&m_ctx)){
        DZY_ASSERT(false);
    }
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;
    m_cb = cb;
    makecontext(&m_ctx, &Fiber::MainFunc,0); 
}

void Fiber::SetThis(Fiber* f){
    t_fiber = f;
}
Fiber::Fiber(){
    m_id = ++s_fiber_id;
    m_state = EXEC;
    if(getcontext(&m_ctx)){
        DZY_ASSERT2(false,"getcontext eroor");
    }
    SetThis(this);
}
void Fiber::YieldToHold(){
    Fiber* cur = GetThis().get();
    cur->m_state = HOLD;
    cur->swapOut();

}
void Fiber::YieldToReady(){

    Fiber* cur = GetThis().get();
    cur->m_state = READY;
    cur->swapOut();
}
Fiber::ptr Fiber::GetThis(){
        if(!t_threadFiber){
            t_threadFiber.reset(new Fiber);
        }
        return t_fiber->shared_from_this();
}

uint64_t Fiber::GetTotal(){
    return s_fiber_count;
}
Fiber::State Fiber::getState() {
    return m_state;
}
void Fiber::MainFunc(){
    Fiber* cur = GetThis().get();
    DZY_ASSERT(cur != nullptr);
    try{
            std::function<void()> cb =cur-> m_cb;
            cur->m_cb = nullptr;
            cb();
    }catch(std::exception& ex){
            cur->m_state = EXCEP;
            DZY_LOG_ERROR(DZY_LOG_ROOT())<<"error : "<<ex.what();
            DZY_ASSERT2(false, "MainFunc error ");
    }catch(...){
            cur->m_state = EXCEP;
            DZY_ASSERT2(false, "MainFunc error ");
    }
    cur->m_state = TEAM;
    cur->swapOut();
    DZY_ASSERT2(false, "never reach");
}
void Fiber::MainFuncFiber(){
    Fiber* cur = GetThis().get();
    DZY_ASSERT(cur != nullptr);
    try{
            std::function<void()> cb =cur-> m_cb;
            cur->m_cb = nullptr;
            cb();
    }catch(std::exception& ex){
            cur->m_state = EXCEP;
            DZY_LOG_ERROR(DZY_LOG_ROOT())<<"error : "<<ex.what();
            DZY_ASSERT2(false, "MainFunc error ");
    }catch(...){
            cur->m_state = EXCEP;
            DZY_ASSERT2(false, "MainFunc error ");
    }
    cur->m_state = TEAM;
    cur->callOut();
    DZY_ASSERT2(false, "never reach");
}


void Fiber::callIn(){
    SetThis(this);
    m_state = EXEC;
   // Fiber* top_fiber = t_fiber;
   int ret = 0;
            DZY_ASSERT2(t_threadFiber, "callIn error");
    if((ret =swapcontext(&t_threadFiber->m_ctx, &Schedule::GetMainFiber()->m_ctx)) != 0){
            DZY_LOG_DEBUG(DZY_LOG_ROOT()) << &t_threadFiber->m_ctx <<"     "<<&t_schedule_fiber->m_ctx << "   "<<ret<<"errno:" <<errno;
            DZY_ASSERT2(false, "callIn error");
    }

}
void Fiber::callOut(){
    t_fiber = t_schedule_fiber.get();
    if(swapcontext(&t_fiber->m_ctx, &t_threadFiber->m_ctx)){
            DZY_ASSERT2(false, "callOut error");
    }
}

}
