#pragma  once

#include "http.h"
#include "http11_common.h"
#include "http11_parser.h"
#include "httpclient_parser.h"


#include <memory>


namespace dzy {
namespace http {
    
class HttpRequestParser {
public:
    typedef std::shared_ptr<HttpRequestParser> ptr;
    
    HttpRequestParser();
    int hasError() ;
    int finish() ;
    size_t execute(char *data, size_t len);
    int isFinished();
    HttpRequest::ptr getData();
    void setError(int error) {m_error = error;}

private:
    HttpRequest::ptr m_data;
    http_parser m_parser;

    int m_error;
};


class HttpResponseParser {
public:
    typedef std::shared_ptr<HttpResponseParser> ptr;

    HttpResponseParser();
    int hasError() ;
    int finish() ;
    size_t execute(char *data, size_t len);
    int isFinished();
    HttpResponse::ptr getData();
    void setError(int error) {m_error = error;}

private:
    HttpResponse::ptr m_data;
    httpclient_parser m_parser;

    int m_error;
};

}
}
