#pragma once

#include <memory>
#include <functional>
#include <map>
#include "http.h"
#include "http_session.h"


namespace dzy {
namespace http{

class Servlet {
public:
    typedef std::shared_ptr<Servlet> ptr;
    Servlet(std::string name):m_name(name){}
    virtual ~Servlet(){
    }

    const std::string& getName()const {return m_name;}
    void setName(const std::string name) {m_name = name;}

    virtual int handle(HttpRequest::ptr req,HttpResponse::ptr rsp,HttpSession::ptr session) = 0;
private:
    std::string m_name;

};

class HttpServlet : public Servlet {
public:
    typedef std::shared_ptr<HttpServlet> ptr;
    typedef std::function<void (HttpRequest::ptr,HttpResponse::ptr,HttpSession::ptr)>  CbType;
    HttpServlet(std::string name,CbType cb);
    virtual int handle(HttpRequest::ptr req,HttpResponse::ptr rsp,HttpSession::ptr session);
private:
    CbType m_cb;
};

class HttpServletDispatch : Servlet {
public:
    typedef std::function<void (HttpRequest::ptr,HttpResponse::ptr,HttpSession::ptr)>  CbType;
    typedef std::shared_ptr<HttpServletDispatch> ptr;
    HttpServletDispatch(std::string name);
    Servlet::ptr getServlet(std::string name);
    void addServlet(std::string name,Servlet::ptr svlt);
    void addServlet(std::string name, CbType cb);
    void delServlet(std::string name);

    virtual int handle(HttpRequest::ptr req,HttpResponse::ptr rsp,HttpSession::ptr session);
private:
    std::map<std::string, Servlet::ptr>  m_svlts;
    Servlet::ptr m_default;

};

}
}
