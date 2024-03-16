#include <asm-generic/errno-base.h>
#include <cerrno>
#include <cstring>
#include <functional>
#include <memory>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include "log.h"
#include "macro.h"
#include "schedule.h"
#include "iomanager.h"



namespace dzy {
void IOManager::fdContext::EventContext::reset(){
        schedule = nullptr;
        fiber.reset();
        cb = nullptr;
}
void IOManager::timerInsertAtFront(){
        tickle();
}

void IOManager::fdContext::trigger(Event event){
//TODO 是否要将EventContext的内容清空执行完之后
    if(event & READ){
        if(read.cb){
            read.schedule->schedule(read.cb);
        }else if(read.fiber){
            read.schedule->schedule(read.fiber);
        }
    }else if(event & WRITE){
        if(write.cb){
            read.schedule->schedule(write.cb);
        }else if(write.fiber){
            read.schedule->schedule(write.fiber);
        }
    }

}
IOManager::IOManager(int num,bool use_caller, std::string name):Schedule(num,use_caller,name){
    m_epfd = epoll_create(5000);
    if(m_epfd <= 0){
        DZY_ASSERT2(false,"epoll_create error");        
    }

    int ret = pipe(m_pipeFd);
    fcntl(m_pipeFd[0], F_SETFL,O_NONBLOCK);
    if(ret != 0){
        DZY_ASSERT2(false ,"pipe error");
    }

    epoll_event eventep;
    memset(&eventep, 0, sizeof(eventep));
    eventep.events = EPOLLET | READ;
    eventep.data.fd =  m_pipeFd[0];

    if(epoll_ctl(m_epfd,EPOLL_CTL_ADD,m_pipeFd[0],&eventep)){
        DZY_ASSERT2(false, "epoll_ctl add error");
    }
    resizeFdCtx(32);
    start();
}
void IOManager::resizeFdCtx(size_t size){
        m_fdCtx.resize(size);
        for(size_t i = 0; i <m_fdCtx.size(); i++){
            if(!m_fdCtx[i]){
                m_fdCtx[i] = new fdContext;
                m_fdCtx[i]->fd = i;
            }
        }
}
IOManager::~IOManager(){
    stop();
    for(size_t i = 0;i < m_fdCtx.size();i++){
        if(m_fdCtx[i]){
            delete m_fdCtx[i];
        }
    }
}
IOManager* IOManager::GetThis(){
    return dynamic_cast<IOManager*>(Schedule::GetThis());
}

IOManager::fdContext::EventContext& IOManager::fdContext::getEventCtx(Event event){
        if(event & READ){
            return read;
        }
        else {
            return write;
        }
}
int  IOManager::addEvent(int fd,Event event,std::function<void()> cb){
    fdContext* fd_ctx = nullptr;
    if(m_fdCtx.size() <= (size_t)fd){
        resizeFdCtx(fd * 2);
        fd_ctx = m_fdCtx[fd];
    }
    fd_ctx = m_fdCtx[fd];
    int op  = fd_ctx->event ? EPOLL_CTL_MOD: EPOLL_CTL_ADD;
    DZY_ASSERT2(!( fd_ctx->event & event),"already add event");
    //TODO Maybe Error
    epoll_event eventep;
    eventep.events = EPOLLET | event | fd_ctx->event;
    //eventep.events = EPOLLET | EPOLLOUT;
    eventep.data.fd = fd;
    eventep.data.ptr = fd_ctx;

    fd_ctx->event = (Event)(fd_ctx->event | event);

    fdContext::EventContext& event_ctx = fd_ctx->getEventCtx(event);
    event_ctx.schedule = Schedule::GetThis();
    if(cb){
        event_ctx.cb.swap(cb);
    }else {
        event_ctx.fiber = Fiber::GetThis();
    }

    if(epoll_ctl(m_epfd,op,fd,&eventep)){
        DZY_LOG_ERROR(DZY_LOG_ROOT())<< "errno = "<<errno <<"  errstr = "<<std::strerror(errno);
        return 0;
    }
    m_pendCount++;
    return 1;
}
bool IOManager::delEvent(int fd,Event event){
        if((size_t)fd >= m_fdCtx.size()){
            return false;
        }
        
        fdContext* fd_ctx = m_fdCtx[fd];
        DZY_ASSERT2(fd_ctx->event & event, "no event delEvent");
        int op = (fd_ctx->event & ~event) ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;

        fd_ctx->event = (Event)(fd_ctx->event & ~event);
        
        fdContext::EventContext& event_ctx = fd_ctx->getEventCtx(event);
       // DZY_ASSERT(!event_ctx.schedule &&
       //            !event_ctx.fiber &&
       //            !event_ctx.cb);

        event_ctx.schedule = nullptr;
        event_ctx.fiber.reset();
        event_ctx.cb = nullptr;
        epoll_event eventep;
        memset(&eventep, 0, sizeof(eventep));
        eventep.events = EPOLLET | fd_ctx->event;
        eventep.data.fd = fd;
        eventep.data.ptr = fd_ctx;

        if(epoll_ctl(m_epfd,op,fd,&eventep)){
            DZY_ASSERT2(false, "epoll_ctl error");
        }
        --m_pendCount;
        return true;
}
bool IOManager::cancleEvent(int fd,Event event){
  if((size_t)fd >= m_fdCtx.size()){
            return false;
        }
        fdContext* fd_ctx = m_fdCtx[fd];
        DZY_ASSERT2(fd_ctx->event & event, "no event delEvent");
        int op = (fd_ctx->event & ~event) ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;

        fd_ctx->event = (Event)(fd_ctx->event & ~event);
        
        fdContext::EventContext& event_ctx = fd_ctx->getEventCtx(event);
        DZY_ASSERT(event_ctx.schedule &&(
                   event_ctx.fiber ||
                   event_ctx.cb));
        
        fd_ctx->trigger(event);

        event_ctx.schedule = nullptr;
        event_ctx.fiber.reset();
        event_ctx.cb = nullptr;

        epoll_event eventep;
        memset(&eventep, 0, sizeof(eventep));
        eventep.events = EPOLLET | fd_ctx->event;
        eventep.data.fd = fd;
        eventep.data.ptr = fd_ctx;

        if(epoll_ctl(m_epfd,op,fd,&eventep)){
            DZY_ASSERT2(false, "epoll_ctl error");
        }
        return true;
}
bool IOManager::cancleAll(int fd){
    if((size_t)fd >= m_fdCtx.size()){
            return false;
        }
        
        fdContext* fd_ctx = m_fdCtx[fd];
        if(fd_ctx->event & READ){       
            fd_ctx->trigger(READ);
            fdContext::EventContext& event_ctx = fd_ctx->getEventCtx(READ);
            event_ctx.reset();
        }

        if(fd_ctx->event & WRITE){       
            fd_ctx->trigger(WRITE);
            fdContext::EventContext& event_ctx = fd_ctx->getEventCtx(WRITE);
            event_ctx.reset();
        }
        fd_ctx->event = NONE;


        epoll_event eventep;
        memset(&eventep, 0, sizeof(eventep));
        eventep.events = EPOLLET | fd_ctx->event;
        eventep.data.fd = fd;
        eventep.data.ptr = fd_ctx;

        if(epoll_ctl(m_epfd,EPOLL_CTL_DEL,fd,&eventep)){
            DZY_ASSERT2(false, "epoll_ctl error");
        }
        return true;
}
void IOManager::tickle(){
    if(hasIdleFiber()){
        int ret = write(m_pipeFd[1], "T", 1);
        DZY_ASSERT(ret == 1);
    }
}
bool IOManager::stopping() {
    return Schedule::stopping()&& m_pendCount == 0 && !hasTimer();
}
void IOManager::idle() {
    epoll_event*  events = new epoll_event[64];
    std::shared_ptr<epoll_event>  events_ptr(events,[](epoll_event* events){
        delete[] events;
    });
    static const uint64_t MAX_TIME = 5000;

    int rt = 0;
    uint64_t next_time = ~0;
    epoll_event event;
    while(true){
        while(true){
            next_time = std::min(getNextTime(),MAX_TIME);
            rt = epoll_wait(m_epfd, events, 64, next_time);
            //TODO if no event return what
            if(rt < 0 && errno == EINTR){
                    continue;
            }else{
                    break;
            }
        }
        //处理timers
        std::vector<std::function<void()> > cbs;
        getExpireCbs(cbs);
        for(auto& i : cbs){
            IOManager::GetThis()->schedule(i);
        }
        //处理读写事件
        for(int i = 0;i < rt;i++){
            event = events[i];
            //if fd is m_pipeFd
            if(event.data.fd == m_pipeFd[0]){
                    int dummpy;
                    int ret = -1;
                    while(true){
                        ret = read(m_pipeFd[0], (void*)&dummpy, sizeof(int));
                        if(ret > 0){
                            continue;
                        }else if(ret < 0 &&  errno == EAGAIN){
                            break;;
                        }else{
                            //error
                        }
                    }
            }else{
                    fdContext* fd_ctx = (fdContext*)event.data.ptr;
                    int real_event = 0;
                    int left_event = 0;
                    left_event = fd_ctx->event & ~real_event;
                    real_event = event.events;
                    //TODO 是否要将EventContext的内容清空执行完之后
                    if(real_event & EPOLLIN && fd_ctx->event & READ){
                        m_pendCount--;
                        fd_ctx->trigger(READ);
//                        fdContext::EventContext& fd_event = fd_ctx->getEventCtx(READ);
//                        if(fd_event.cb){
//                            fd_event.schedule->schedule(fd_event.cb);
//                        }else{
//                            fd_event.schedule->schedule(fd_event.fiber);
//                        }
                    }
                    //TODO 是否要将EventContext的内容清空执行完之后
                    if(real_event & EPOLLOUT && fd_ctx->event & WRITE){
                        m_pendCount--;
                        fd_ctx->trigger(WRITE);
//                        fdContext::EventContext& fd_event = fd_ctx->getEventCtx(WRITE);
//                        if(fd_event.cb){
//                            fd_event.schedule->schedule(fd_event.cb);
//                        }else{
//                            fd_event.schedule->schedule(fd_event.fiber);
//                        }
                    }
                    if((real_event & fd_ctx->event) == NONE){
                        continue;
                    }
                    
                    int op = left_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
                    event.events = EPOLLET | left_event;
                    if(epoll_ctl(m_epfd, op, fd_ctx->fd, &event)){
                        DZY_ASSERT2(false, "epoll_ctl error");
                    }
                    fd_ctx->event = (Event)left_event;
            }
    }

    Fiber::YieldToHold();
}

}

}
