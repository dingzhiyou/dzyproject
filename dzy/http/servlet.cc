#include "servlet.h"
namespace dzy{
namespace http {


HttpServlet::HttpServlet(std::string name,CbType cb):Servlet(name)
                                    ,m_cb(cb){
}

static char not_found[] =   "<!DOCTYPE html>"
                            "<html lang=\"en\">"
                            "<head>"
                            "<meta charset=\"UTF-8\">"
                            "<title>404 Not Found</title>"
                            "</head>"
                            "<body>"
                            "<h1>404 Not Found</h1>"
                            "<p>The page you requested could not be found.</p>"
                            "</body>"
                            "</html>";
void  default_fun(HttpRequest::ptr req,HttpResponse::ptr rsp,HttpSession::ptr session) {
    std::string body = not_found;
    rsp->setBody(body);
    
}
int HttpServlet::handle(HttpRequest::ptr req,HttpResponse::ptr rsp,HttpSession::ptr session) {
     m_cb(req,rsp,session);
     return 0;
}

HttpServletDispatch::HttpServletDispatch(std::string name):Servlet(name) {
    m_default.reset(new HttpServlet("Not FOUND",default_fun));
}
Servlet::ptr HttpServletDispatch::getServlet(std::string name) {
    auto it = m_svlts.find(name);
    if(it != m_svlts.end()){
        return it->second;
    }
    return m_default;
}
void HttpServletDispatch::addServlet(std::string name,Servlet::ptr svlt) {
    m_svlts[name] = svlt;
}
void HttpServletDispatch::addServlet(std::string name, CbType cb) {
    HttpServlet::ptr svlt(new HttpServlet(name,cb));
    m_svlts[name] = svlt;
}
void HttpServletDispatch::delServlet(std::string name){
    m_svlts.erase(name);
}

int HttpServletDispatch::handle(HttpRequest::ptr req,HttpResponse::ptr rsp,HttpSession::ptr session) {
    std::string path = req->getPath();
    Servlet::ptr svlt = getServlet(path);
    svlt->handle(req,rsp,session);
    return 0;
}

}
}
