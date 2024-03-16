#include "http.h"
#include <sstream>
#include <string.h>
#include <string>


namespace dzy {
namespace http{

HttpMethod StringToHttpMethod(std::string& m){
#define XX(num,name,string)  \
    if(strcmp(#name,m.c_str()) == 0){ \
        return HttpMethod::name;  \
    } 
    HTTP_METHOD_MAP(XX);
#undef XX
    return HttpMethod::INVILD_METHOD;
}
HttpMethod CharsToHttpMethod(const char* m){
#define XX(num,name,string)  \
    if(strncmp(#name,m,strlen(#name))== 0){ \
        return HttpMethod::name;  \
    } 
    HTTP_METHOD_MAP(XX);
#undef XX
    return HttpMethod::INVILD_METHOD;
}
std::string HttpMethodToString(HttpMethod& h){
#define XX(num,name,string) \
     if(h == HttpMethod::name){ \
         return #name; \
     }  
    HTTP_METHOD_MAP(XX);
#undef XX
    return "<UNKNOW-METHOD>";
}
std::string HttpStatusToString(HttpStatus& h) {
#define XX(code,name,discription) \
    if(h == HttpStatus::name){ \
        return #name;  \
    }
    HTTP_STATUS_MAP(XX);
#undef XX
    return "UNKNOW-STATUS";
}


bool CaseInSensitiveCompare::operator()(std::string lstr,std::string rstr){
    return ((strcasecmp(lstr.c_str(),rstr.c_str())) < 0);
}


HttpRequest::HttpRequest(uint8_t version, bool close ):m_version(version)
                                                            ,m_close(close)
                                                            ,m_path("/"){
            m_method = HttpMethod::GET;
}


void HttpRequest::addHeader(std::string key,std::string val){
    m_headers[key] = val;
}
void HttpRequest::addParam(std::string key,std::string val){
    m_params[key] = val;
}
void HttpRequest::addCookie(std::string key,std::string val){
    m_cookies[key] = val;
}

void HttpRequest::delHeader(std::string key){
    m_headers.erase(key);
}
void HttpRequest::delParam(std::string key){
    m_params.erase(key);
}
void HttpRequest::delCookie(std::string key){
    m_cookies.erase(key);
}


void HttpRequest::getHeader(std::string key,std::string* val){
    auto it = m_headers.find(key);
    if(it != m_headers.end()){
        *val = it->second;
    }
}
void HttpRequest::getParam(std::string key,std::string* val){
    auto it = m_params.find(key);
    if(it != m_params.end()){
        *val = it->second;
    }

}
void HttpRequest::getCookie(std::string key,std::string* val){
    auto it = m_cookies.find(key);
    if(it != m_cookies.end()){
        *val = it->second;
    }
}

std::string HttpRequest::toString(){
    std::stringstream os;
    dump(os);
    return os.str();
}
void HttpRequest::dump(std::stringstream& os){
    std::string version;
    std::stringstream ss;
    if(m_version == 0x11){
        version = "1.1";
    }else{
        version = "1.0";
    }

    if(m_close){
        addHeader("connect", "close");
    }else{
        addHeader("connect", "keep-alive");
    }
    if(!m_body.empty()){
        addHeader("Content-length", std::to_string(m_body.size()));
    }
    for(auto i : m_headers){
        ss<<i.first<<": "<<i.second<<"\r\n";
    }
    std::ostringstream path;
    path << m_path;
    if(!m_query.empty()){
        path<<"?"<<m_query;
    }
    if(!m_fragment.empty()){
        path <<"#"<<m_fragment;
    }


    os <<HttpMethodToString(m_method) <<" "<<path.str()<<" "
       <<"HTTP/"<<version 
       <<"\r\n"
       <<ss.str() <<"\r\n"
       <<m_body;
}


HttpResponse::HttpResponse(uint8_t v,HttpStatus s,bool close):m_version(v)
                                                             ,m_status(s)
                                                             ,m_close(close){
}
void HttpResponse::addHeader(std::string key,std::string val){
    m_headers[key] = val;
}
void HttpResponse::getHeader(std::string key,std::string* val ){
    auto i = m_headers.find(key);
    if(i != m_headers.end()){
        *val = i->second;
    }
}
void HttpResponse::delHeader(std::string key){
    m_headers.erase(key);
}

void HttpResponse::dump(std::stringstream& os){
    std::string version;
    std::stringstream headers;

    if(m_version == 0x11){
        version = "1.1";
    }else{
        version = "1.0";
    }
   if(m_close){
        addHeader("connect", "close");
    }else{
        addHeader("connect", "keep-alive");
    }
    if(!m_body.empty()){
        addHeader("Content-length", std::to_string(m_body.size()));
    }
    for(auto i : m_headers){
        headers<<i.first<<": "<<i.second <<"\r\n";
    }
 
    os <<"HTTP/"<<version
       <<" "<<(uint32_t)m_status
       <<" "<<HttpStatusToString(m_status)
       <<"\r\n"
       <<headers.str()
       <<"\r\n"<<m_body;
}
std::string HttpResponse::toString(){
    std::stringstream os;
    dump(os);
    return os.str();
}

std::ostream& operator<<(std::ostream& os,HttpRequest::ptr req){
    return os<<req->toString();
}
std::ostream& operator<<(std::ostream& os,HttpResponse::ptr rsp){
    return os << rsp->toString();
}

}
}
