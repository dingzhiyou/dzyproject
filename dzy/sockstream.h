#include "socket.h"
#include <memory>



namespace dzy {

class SocketStream {
public:
    typedef std::shared_ptr<SocketStream> ptr;
    SocketStream(Socket::ptr sock,bool owner = true);
    virtual ~SocketStream();

    virtual int read(void* buf, size_t len) ;
    virtual int write(void* buf,size_t len) ;

    virtual int readFixSize(void* buf, size_t len);
    virtual int writeFixSize(void* buf,size_t len);
private:
    Socket::ptr m_sock;
    bool m_owner;
};

}
