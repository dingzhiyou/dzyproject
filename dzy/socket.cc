#include "socket.h"
#include "addr.h"
#include "fd_manager.h"
#include <asm-generic/socket.h>
#include <bits/types/struct_timeval.h>
#include <cerrno>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/uio.h>
#include "hook.h"
#include "log.h"

namespace dzy {
static Logger::ptr g_logger = DZY_LOG_ROOT();
Socket::Socket(int family, int type, int proto):m_family(family)
                                               ,m_type(type)
                                               ,m_proto(proto)
                                               ,m_socket(-1)
                                               ,m_error(0){

}


Socket::ptr Socket::CreateTCP(){
        Socket::ptr sock(new Socket(AF_INET,SOCK_STREAM,0));
        int sockfd = ::socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
        sock->setSocket(sockfd);
        sock->init();
        return sock;
}
Socket::ptr Socket::CreateUDP() {
        Socket::ptr sock(new Socket(AF_INET,SOCK_DGRAM,0));
        int sockfd = ::socket(AF_INET,SOCK_DGRAM, 0);
        sock->setSocket(sockfd);
        sock->init();
        return sock;
}

int64_t Socket::getSendTimeout() const{
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(m_socket);
    if(ctx){
        return ctx->getTimeOut(2);
    }
    return -1;
}
int Socket::setSendTimeout(int64_t val){
    struct timeval tv{val / 1000,val % 1000 / 1000};
    auto fd_ctx = FdMgr::GetInstance()->get(m_socket);
    fd_ctx->setTimeOut(2, val);
    return setOption(SOL_SOCKET,SO_SNDTIMEO,tv);
}
int64_t Socket::getRecvTimeout()const {
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(m_socket);
    if(ctx){
        return ctx->getTimeOut(1);
    }
    return -1;
}
int Socket::setRecvTimeout(int64_t val){
    struct timeval tv{val / 1000,val % 1000 / 1000};
    auto fd_ctx = FdMgr::GetInstance()->get(m_socket);
    fd_ctx->setTimeOut(1, val);
    return setOption(SOL_SOCKET,SO_RCVTIMEO,tv);
}

int Socket::getOption(int level,int opt,void* optval, socklen_t* length){
    int rt = getsockopt(m_socket,level,opt,optval,length);
    if(rt){
        DZY_LOG_ERROR(g_logger) << "getsockopt error="<<errno<<"errstrig:"<<strerror(errno);
    }
    return rt;
}
int Socket::setOption(int level,int opt,void* optval,socklen_t* length){
    int rt = setsockopt(m_socket,level,opt,optval,*length);
    if(rt){
        DZY_LOG_ERROR(g_logger) << "setsockopt error="<<errno<<"errstrig:"<<strerror(errno);
    }
    return rt;
}
Socket::ptr Socket::accept(){
    if(m_socket != -1){
        int sock = ::accept(m_socket,nullptr,nullptr);
        if(sock > 0){
            return newSocket(sock);
        }
    }
    return nullptr;
}
int Socket::connect(const Addreess::ptr addr){
        int rt = ::connect(m_socket,addr->getAddr(),addr->getAddrLen());
        if(rt){
            DZY_LOG_ERROR(g_logger) << "connect error="<<errno<<"errstrig:"<<strerror(errno);
            return -1;
        }
        init();
        return rt;
}

int Socket::close(){
    return ::close(m_socket);
}
int Socket::bind(const Addreess::ptr addr){
    DZY_LOG_DEBUG(g_logger) <<"xxxxxxxxxxxxxxxxxxxx="<<ntohs(((struct sockaddr_in*)addr->getAddr())->sin_port);
    int rt = ::bind(m_socket,addr->getAddr(),addr->getAddrLen());
    if(rt){
            DZY_LOG_ERROR(g_logger) << "bind error="<<errno<<"errstrig:"<<strerror(errno) <<"  addr="<<addr->toString();
    }
    return rt;
}
int Socket::listen(int backlog ){
    int rt = ::listen(m_socket, backlog);
    if(rt){
            DZY_LOG_ERROR(g_logger) << "listen error="<<errno<<"errstrig:"<<strerror(errno);
    }
    return rt;
}
ssize_t Socket::recv(void *buf, size_t len, int flags){
        return ::recv(m_socket,buf,len,flags);
}
ssize_t Socket::recv(struct iovec* buf, size_t len, int flags){
    return ::readv(m_socket,buf,len);
}
ssize_t Socket::recvfrom(void *buf, size_t len, Addreess::ptr addr,int flags){
    socklen_t length = addr->getAddrLen();
    return ::recvfrom(m_socket,buf,len,flags,addr->getAddr(),&length);
}
//ssize_t Socket:recvfrom(struct iovec* buf, size_t len, Addreess::ptr addr,int flags){
//}
ssize_t Socket::send(void *buf, size_t len, int flags){
    return ::send(m_socket,buf,len,flags);
}
ssize_t Socket::send(struct iovec* buf, size_t len, int flags){
    return ::writev(m_socket,buf,len);
}
ssize_t Socket::sendto(void *buf, size_t len, Addreess::ptr addr,int flags){
    socklen_t length = addr->getAddrLen();
    return ::sendto(m_socket,buf,len,flags,addr->getAddr(),length);
}
//ssize_t Socket::sendto(struct iovec* buf, size_t len, Addreess::ptr addr,int flags);
Addreess::ptr Socket::getRemoteAddress(){
    if(m_remoteAddress){
        return m_remoteAddress;
    }
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);
    if (getpeername(m_socket, reinterpret_cast<struct sockaddr*>(&remote_addr), &remote_addr_len) == -1) {
        perror("getpeername() failed");
        return nullptr;
    }
    return IPv4Address::Create(&remote_addr,remote_addr.sin_port);
}

Addreess::ptr Socket::getSockAddress(){
    struct sockaddr_in local_addr;
    socklen_t local_addr_len = sizeof(local_addr);
    if (getpeername(m_socket, reinterpret_cast<struct sockaddr*>(&local_addr), &local_addr_len) == -1) {
        perror("getsockname() failed");
        return nullptr;
    }
    return IPv4Address::Create(&local_addr,local_addr.sin_port);
}
bool Socket::isSocket(){
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(m_socket);
    return ctx->isSocket();
}
bool Socket::init(){
    getSockAddress();
    getRemoteAddress();
    int enable = 1;
    setOption(SOL_SOCKET,SO_REUSEADDR,enable);
    return true;
}
Socket::ptr Socket::newSocket(int sock){
    Socket::ptr socket(new Socket(m_family,m_type,m_proto));
    socket->setSocket(sock);
    socket->init();
    return socket;
}
void Socket::setSocket(int sock){
    m_socket = sock;
}


}
