#include "http_server.h"
#include "http.h"
#include "http_session.h"
#include "servlet.h"

namespace dzy {
namespace http {

static Logger::ptr g_logger = DZY_LOG_ROOT();

HttpServer::HttpServer() {
    m_dispatch.reset(new HttpServletDispatch("dispatch"));
}
void HttpServer::handleClient(Socket::ptr client) {
    HttpSession::ptr session(new HttpSession(client));
    HttpRequest::ptr req = session->recvRequest();
    HttpResponse::ptr rsp(new HttpResponse);
    m_dispatch->handle(req,rsp,session);
    if(!req){
        DZY_LOG_WARN(g_logger) <<"req is nullptr";
        return ;
    }
    
    std::string rsp_string = rsp->toString();
    DZY_LOG_INFO(g_logger) << "rsp_string="<<rsp_string;
    client->send(&rsp_string[0],rsp_string.size());

}

}
}
