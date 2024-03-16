#include "fd_manager.h"
#include "log.h"
#include "singleton.h"
#include <asm-generic/errno-base.h>
#include <cerrno>
#include  <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>

namespace dzy {

FdCtx::FdCtx(int fd):m_fd(fd){
    init();
}

void FdCtx::init(){
    if(m_isinit){
        return;
    }
    struct stat st;
    if(-1 !=fstat(m_fd, &st)){
        if(!S_ISSOCK(st.st_mode)){
            DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "not socket="<<m_fd;
            m_issocket = false;
            m_userNonblock = true;
        }else{
            m_issocket = true;
            fcntl(m_fd, F_SETFL,O_NONBLOCK);
            m_sysNonblock = true;
        }
        m_isinit = true;
    }

}
bool FdCtx::isSocket(){
    return m_issocket;
}

bool FdCtx::isClose(){
    int flag = fcntl(m_fd,F_GETFL); 
    if(flag == -1 && errno == EBADF){
        return true;
    }
    return false;
}
uint64_t FdCtx::getTimeOut(int type){
    if(type == 1){
        return m_recvTimeOut;
    }else if(type == 2){
        return m_sendTimeOut;
    }
    return -1;
}
void FdCtx::setTimeOut(int type,uint64_t timeout){
    if(type == 1){
        m_recvTimeOut = timeout;
    }else if(type == 2){
        m_sendTimeOut = timeout;
    }
    
}


FdCtx::ptr FdManager::get(int fd,bool auto_create){
    if(fd < (int)m_fdCtx.size() && m_fdCtx[fd]){
        return m_fdCtx[fd];
    }else if(fd < (int)m_fdCtx.size() && !m_fdCtx[fd]){
        if(auto_create == true){
        m_fdCtx[fd].reset(new FdCtx(fd));
        return m_fdCtx[fd];
    }
    }
    if(auto_create == true){
        if((int)m_fdCtx.size() <= fd){
            m_fdCtx.resize(2 * fd);
        }
        m_fdCtx[fd].reset(new FdCtx(fd));
        return m_fdCtx[fd];
    }
    return m_fdCtx[fd];
}
bool FdManager::del(int fd){
    if(fd < (int)m_fdCtx.size()){
        m_fdCtx[fd].reset();
        return true;
    }
    return false;
}
typedef Singleton<FdManager> FdMgr;

}
