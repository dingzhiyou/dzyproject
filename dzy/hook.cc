#include "hook.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cerrno>
#include <dlfcn.h>
#include <fcntl.h>
#include "fiber.h"
#include <memory>
#include <utility>
#include "fd_manager.h"
#include "fiber.h"
#include "iomanager.h"
#include "log.h"
#include "schedule.h"

#define HOOK_FUN(XX)  \
    XX(sleep) \
    XX(usleep) \
    XX(read) \
    XX(readv) \
    XX(recv) \
    XX(recvfrom) \
    XX(recvmsg) \
    XX(write) \
    XX(writev) \
    XX(send) \
    XX(sendto) \
    XX(sendmsg) \
    XX(getsockopt) \
    XX(setsockopt) \
    XX(accept) \
    XX(connect)

static thread_local bool t_isHook = false;
namespace dzy{

bool isHook(){
    return t_isHook;
}
void setHook(bool flag){
    t_isHook = flag;
}


struct _Hook_init {
    _Hook_init();
};
_Hook_init::_Hook_init(){
#define XX(name)  name ## _f =  (name ## _fun)dlsym(RTLD_NEXT , #name);
    HOOK_FUN(XX);
#undef XX
}

static _Hook_init s_hook_init;

struct Cond {
int  error = 0;
};
template<class OriginFun, typename...  Args>
int do_io(OriginFun fun,const char* hook_name,int fd, dzy::IOManager::Event event,int64_t timeout ,Args&&... args){
        if(!dzy::isHook()){
            return fun(fd,std::forward<Args>(args)...);
        }
        if(fd == 4 ||  fd == 5){
            return fun(fd,std::forward<Args>(args)...);
        }
        auto fd_ctx = FdMgr::GetInstance()->get(fd,true);
        //fd_ctx->setTimeOut(1, 2000);
        //fd_ctx->setTimeOut(2, 2000);
        if(fd_ctx->isClose()){ 
                DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "do_io="<<fd<<" hook_name="<<hook_name<<" fd=:"<<fd;
                errno = EBADF;
        }
        if(!fd_ctx->isSocket()){
            return fun(fd,std::forward<Args>(args)...);
        }
        if(timeout == -1){
            if(event & dzy::IOManager::READ){
                timeout = fd_ctx->getTimeOut(1);
            }         
            if(event & dzy::IOManager::WRITE){
                timeout = fd_ctx->getTimeOut(2);
            }
        }
        std::shared_ptr<dzy::Cond> cond(new dzy::Cond);
        int ret = 0;
retry:
        ret = fun(fd,std::forward<Args>(args)...);
        while(ret == -1 && errno == EINTR){
            ret = fun(fd,std::forward<Args>(args)...);
        }
        if(ret == -1 && errno == EAGAIN){
            std::weak_ptr<dzy::Cond> weak_cond(cond); 
            IOManager* iom = IOManager::GetThis();
            auto timer =  iom->addTimerCondition(timeout,[weak_cond,fd,iom,event](){
                    std::shared_ptr<dzy::Cond> cond = weak_cond.lock();
                    if(!cond){
                        return;
                    }
                    cond->error = ETIMEDOUT;
                    iom->cancleEvent(fd, event);
            },weak_cond);
            int rt = iom->addEvent(fd,event);
            if(!rt){
                if(timer){
                    timer->cancle();
                }
                return -1;
            }else{
                Fiber::YieldToHold();
                if(timer){
                    timer->cancle();
                }
                if(cond->error){
                    errno = cond->error;
                    return -1;
                }else {
                    iom->delEvent(fd, event);
                }
goto retry;
            }
        }
        return ret;
}
}

extern "C"{
#define XX(name)  name ##  _fun   name  ## _f = nullptr;
    HOOK_FUN(XX);
#undef XX

unsigned int sleep(unsigned int seconds){
    if(!dzy::isHook()){
        return sleep_f(seconds);
    }
    dzy::Fiber::ptr fiber = dzy::Fiber::GetThis();
    dzy::IOManager* iom = dzy::IOManager::GetThis();   

    iom->addTimer(seconds * 1000,[iom,fiber](){
        iom->schedule(fiber);
    });

    dzy::Fiber::YieldToHold();
    return 0;
}

int usleep(__useconds_t __useconds){
    if(!dzy::isHook()){
        return usleep_f(__useconds);
    }
    dzy::Fiber::ptr fiber = dzy::Fiber::GetThis();
    dzy::IOManager* iom = dzy::IOManager::GetThis();   

    iom->addTimer(__useconds /  1000,[iom,fiber](){
        iom->schedule(fiber);
    });

    dzy::Fiber::YieldToHold();
    return 0;
}



//read
ssize_t read(int fd, void *buf, size_t count){
    return do_io(read_f,"read",fd,dzy::IOManager::READ,-1,buf,count);
}

ssize_t readv(int fd, const struct iovec *iov, int iovcnt){
    return do_io(readv_f,"readv",fd,dzy::IOManager::READ,-1,iov,iovcnt);
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags){
    return do_io(recv_f,"recv",sockfd,dzy::IOManager::READ,-1,buf,len,flags);
}

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen){
    return do_io(recvfrom_f,"recvfrom",sockfd,dzy::IOManager::READ,-1,buf,len,flags,src_addr,addrlen);
}

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags){
    return do_io(recvmsg_f,"recvmsg",sockfd,dzy::IOManager::READ,-1,msg,flags);
}


//write
ssize_t writev(int fd, const struct iovec *iov, int iovcnt){
    return  do_io(writev_f,"writev",fd,dzy::IOManager::WRITE,-1, iov,iovcnt);
}

ssize_t write(int fd, const void *buf, size_t count){
    return do_io(write_f,"write",fd,dzy::IOManager::WRITE,-1,buf,count);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags){
    return do_io(send_f,"send",sockfd,dzy::IOManager::WRITE,-1,buf,len,flags);
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen){
    return do_io(sendto_f,"sendto",sockfd,dzy::IOManager::WRITE,-1,buf,len,flags,dest_addr,addrlen);
}

ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags){
    return do_io(sendmsg,"sendmsg_f",sockfd,dzy::IOManager::WRITE,-1,msg,flags);
}

//fcntl
//int fcntl(int fd, int cmd, ... /* arg */ ){
//socket 
int getsockopt(int sockfd, int level, int optname,
          void *optval, socklen_t *optlen){
    return getsockopt_f(sockfd, level, optname, optval, optlen);
}

int setsockopt(int sockfd, int level, int optname,
          const void *optval, socklen_t optlen){
    return setsockopt_f(sockfd, level, optname, optval, optlen);
}
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
    int fd = do_io(accept_f,"accept",sockfd,dzy::IOManager::READ,-1,addr,addrlen);
    if(fd != -1){
        dzy::FdMgr::GetInstance()->get(fd,true); 
    }
    return fd;
}

int connect_with_timeout(int sockfd, const struct sockaddr *addr, socklen_t addrlen, uint64_t timeout){
     if(!dzy::isHook()){
        return connect_f(sockfd,addr,addrlen);
    }

    auto fd_ctx = dzy::FdMgr::GetInstance()->get(sockfd, true);
    if(fd_ctx->getUserNonblock()){
        return connect_f(sockfd,addr,addrlen);
    }
    if(fd_ctx->isClose() || !fd_ctx->isSocket()){
        return connect_f(sockfd,addr,addrlen);
    }
retry:
    int fd = connect_f(sockfd,addr,addrlen);
    if(fd  == 0){
        return 0;
    }else if(fd != -1 || errno != EINPROGRESS){
        return fd;
    }
    dzy::IOManager* iom = dzy::IOManager::GetThis();
    std::shared_ptr<dzy::Cond> cond(new dzy::Cond);
    std::weak_ptr<dzy::Cond> weak_cond(cond);

    auto timer = iom->addTimerCondition(timeout, [weak_cond,iom,sockfd](){
            std::shared_ptr<dzy::Cond> cond = weak_cond.lock();
            if(!cond || cond->error != -1){
                return;
            }
            cond->error = ETIMEDOUT;
            iom->cancleEvent(sockfd, dzy::IOManager::WRITE);
    }, weak_cond);
    int rt = iom->addEvent(sockfd, dzy::IOManager::WRITE);
    if(rt == 1){
        dzy::Fiber::YieldToHold();
        if(timer){
            timer->cancle();
        }
        if(cond && cond->error != -1){
            errno = cond->error;
            return -1;
        }
goto retry;
    }else if(rt == 0){
        if(timer){
            timer->cancle();
        }
        DZY_LOG_DEBUG(DZY_LOG_ROOT()) <<"addEvent error";
    }
    return -1;
}
int connect(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen){
    return connect_f(sockfd,addr,addrlen);
}

}
