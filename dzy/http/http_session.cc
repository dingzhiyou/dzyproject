#include "http_session.h"
#include "http_parser.h"
#include "../log.h"
#include <exception>
#include <string.h>
#include <boost//lexical_cast.hpp>

namespace dzy {
namespace  http {
static Logger::ptr g_logger = DZY_LOG_ROOT();

HttpSession::HttpSession(Socket::ptr client,bool owner):SocketStream(client,owner){

}
HttpSession::~HttpSession(){

}
HttpRequest::ptr HttpSession::recvRequest(){
    HttpRequestParser::ptr parser(new HttpRequestParser);
    size_t buff_len = 1024 * 1024;
    std::shared_ptr<char> buff(new char[buff_len],[](char* ptr){
        delete[] ptr;
    });
    
    char* buf = buff.get();
    size_t offset = 0;
    do{
        int n  = read(buf + offset, buff_len - offset);
        if(n == 0){
            DZY_LOG_INFO(g_logger) <<"client disconnet";
            return nullptr;
        }else if(n == -1){
            DZY_LOG_INFO(g_logger) <<"client read error, errno="<<errno <<"  errstr="<<strerror(errno);
            return nullptr;
        }
        int nparser = parser->execute(buf, offset);
        offset -= nparser;
        if(parser->hasError()){
            DZY_LOG_INFO(g_logger) <<"parser->execute has error,errno="<<errno 
                                   << "  errstr="<<strerror(errno);
            return nullptr;
        }
        if(parser->isFinished()){
            break;
        }
    }while(true);
    std::string body;
    std::string length;
    int len;
    parser->getData()->getHeader("content-length",&length);
    try{
        len = boost::lexical_cast<int>(length);
    }catch(std::exception& e){
            DZY_LOG_WARN(g_logger) <<"e.what="<<e.what();
            return nullptr;
    }
    
    body.reserve(len);
    body.append(buf,offset);
    int left = len - offset;
    readFixSize(buf + offset, left);

    parser->getData()->setBody(body);
    return parser->getData();
}

void HttpSession::sendResponse(HttpResponse::ptr rsp){
    std::string str = rsp->toString();
    writeFixSize(&str[0],str.size());
}

}
}
