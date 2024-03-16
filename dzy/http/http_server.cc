#include "http_server.h"
#include "http.h"
#include "http_session.h"

namespace dzy {
namespace http {

static Logger::ptr g_logger = DZY_LOG_ROOT();

void HttpServer::handleClient(Socket::ptr client) {
    HttpSession::ptr session(new HttpSession(client));
    DZY_LOG_WARN(DZY_LOG_ROOT()) <<"i am in handleClient";   
    HttpRequest::ptr req = session->recvRequest();

    if(!req){
        DZY_LOG_WARN(g_logger) <<"req is nullptr";
        return ;
    }
    std::string rsp = req->toString();

    DZY_LOG_INFO(g_logger) << "rsp="<<rsp;
    client->send(&rsp[0],rsp.size(),0);


}

}
}
