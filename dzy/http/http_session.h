#pragma once 
#include "../sockstream.h"
#include "http.h"

namespace dzy {
namespace http {

class HttpSession : public SocketStream {
public:
    typedef std::shared_ptr<HttpSession> ptr;
    
    HttpSession(Socket::ptr client,bool onwer = true);
    ~HttpSession();
    HttpRequest::ptr recvRequest();
    void sendResponse(HttpResponse::ptr rsp);

private:


};

}
}
