#pragma once

#include <bits/stdint-intn.h>
#include <bits/types/struct_iovec.h>
#include <memory>
#include <sys/socket.h>
#include <sys/types.h>
#include "addr.h"

namespace dzy {

class Socket : public std::enable_shared_from_this<Socket> {
public:
    typedef std::shared_ptr<Socket> ptr;

    Socket(int family, int type, int proto = 0);
    static Socket::ptr CreateTCP();
    static Socket::ptr CreateUDP();

    int64_t getSendTimeout() const;
    int setSendTimeout(int64_t val);
    int64_t getRecvTimeout()const;
    int setRecvTimeout(int64_t val);


    int getOption(int level,int opt,void* optval, socklen_t* length);
    template<class T>
    int getOption(int level,int opt, T& result) {
        socklen_t length = sizeof(T);
        return getOption(level,opt,&result,&length);
    }

    int setOption(int level,int opt,void* optval,socklen_t* length);
    template<class T>
    int setOption(int level,int opt,T& result){
        socklen_t length = sizeof(result);
        return setOption(level,opt,&result,&length);
    }


    Socket::ptr accept();

    int connect(const Addreess::ptr addr);
    int bind(const Addreess::ptr addr);
    int listen(int backlog = 5000);
    int close();

    ssize_t recv(void *buf, size_t len, int flags = 0);
    ssize_t recv(struct iovec* buf, size_t len, int flags = 0);
    ssize_t recvfrom(void *buf, size_t len, Addreess::ptr addr,int flags = 0);
    //ssize_t recvfrom(struct iovec* buf, size_t len, Addreess::ptr addr,int flags);

    ssize_t send(void *buf, size_t len, int flags = 0);
    ssize_t send(struct iovec* buf, size_t len, int flags = 0);
    ssize_t sendto(void *buf, size_t len, Addreess::ptr addr,int flags = 0);
    //ssize_t sendto(struct iovec* buf, size_t len, Addreess::ptr addr,int flags);

    Addreess::ptr getRemoteAddress();
    Addreess::ptr getSockAddress();

    bool isSocket();
    void setSocket(int sock);
private:
    bool init();
    Socket::ptr newSocket(int sock);


private:
    int m_family;
    int m_type;
    int m_proto;
    int m_socket;
    int m_error;

    Addreess::ptr m_sockAddress;
    Addreess::ptr m_remoteAddress;
    
};

}
