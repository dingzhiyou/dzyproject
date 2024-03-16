#include "sockstream.h"
#include "log.h"
#include <cerrno>
#include <string.h>


namespace dzy {

static Logger::ptr g_logger = DZY_LOG_ROOT();

SocketStream::SocketStream(Socket::ptr sock,bool owner ):m_sock(sock)
                                                        ,m_owner(owner){

}
SocketStream::~SocketStream() {
    if(m_owner){
        m_sock->close();
    }
}

int SocketStream::read(void* buf, size_t len) {
    int s = m_sock->recv(buf,len,0);
    return s;
}
int SocketStream::write(void* buf,size_t len)  {
    int s= m_sock->send(buf,len,0);
    return s;

}
int SocketStream::readFixSize(void* buf, size_t len) {
    size_t offset = 0;
    while(len >  offset) {
        int n = read((char*)buf + offset, len - offset);
        if(n == 0){
            DZY_LOG_INFO(g_logger) << "Clinet disconnect";
            return offset;;
        }else if(n == -1) {
            DZY_LOG_INFO(g_logger) << "readFixSize errno=" <<errno <<"  errstr=" << strerror(errno);
            return -1;
        }
        offset += n;
    }
    return len;
}
int SocketStream::writeFixSize(void* buf,size_t len) {
    size_t offset = 0;
    while(len > offset) {
        int n = write((char*)buf + offset, len - offset);
        if(n == 0){
            DZY_LOG_INFO(g_logger) << "Clinet disconnect";
            return offset;;
        }else if(n == -1) {
            DZY_LOG_INFO(g_logger) << "writeFixSize errno=" <<errno <<"  errstr=" << strerror(errno);
            return -1;
        }
        offset += n;
    }
    return len;
}

}
