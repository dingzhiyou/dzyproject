#include "http_parser.h"
#include "http.h"
#include "http11_parser.h"
#include "httpclient_parser.h"
#include "../../dzy/log.h"


#include <cstdlib>
#include <ctime>
#include <string.h>
namespace dzy {
namespace http {
static Logger::ptr g_logger = DZY_LOG_ROOT();

void on_request_method(void *data, const char *at, size_t length) { 
    HttpRequestParser* parser= static_cast<HttpRequestParser*>(data);
    HttpMethod m = CharsToHttpMethod(at);
    if(m == HttpMethod::INVILD_METHOD){
        parser->setError(1000);
        DZY_LOG_WARN(g_logger) <<"inviled method=" <<std::string(at,length);
        return;

    }
    parser->getData()->setMethod(m);
}
void on_request_uri(void *data, const char *at, size_t length) { 
    //HttpRequest* req = static_cast<HttpRequest*>(data);

}
void on_request_path(void *data, const char *at, size_t length) { 
    HttpRequestParser* parser= static_cast<HttpRequestParser*>(data);
    parser->getData()->setPath(std::string(at,length));
}
void on_request_fragement(void *data, const char *at, size_t length) { 
    HttpRequestParser* parser= static_cast<HttpRequestParser*>(data);
    parser->getData()->setFragment(std::string(at,length));
}
void on_request_query_string(void *data, const char *at, size_t length) { 
    HttpRequestParser* parser= static_cast<HttpRequestParser*>(data);
    parser->getData()->setQuery(std::string(at,length));
}
void on_request_http_version(void *data, const char *at, size_t length) { 
    HttpRequestParser* parser= static_cast<HttpRequestParser*>(data);
    //TODO
    DZY_LOG_INFO(g_logger) <<std::string(at,length);
    //if(atoi(at);
    if(std::string(at,length) == std::string("HTTP/1.1")){
        parser->getData()->setVersion(0x11);
    }else {
        parser->getData()->setVersion(0x10);
    }
}
void on_request_header_done(void *data, const char *at, size_t length) { 
    //HttpRequestParser* parser= static_cast<HttpRequestParser*>(data);
}

void on_request_http_field(void *data, const char *field, size_t flen, const char *value, size_t vlen){
    HttpRequestParser* parser= static_cast<HttpRequestParser*>(data);
    parser->getData()->addHeader(std::string(field,flen), std::string(value,vlen));
}
//request
HttpRequestParser::HttpRequestParser() :m_error(0){
    m_data.reset(new HttpRequest);
    http_parser_init(&m_parser);

    m_parser.request_method = on_request_method;
    m_parser.request_uri = on_request_uri;
    m_parser.request_path = on_request_path;
    m_parser.fragment = on_request_fragement;
    m_parser.query_string = on_request_query_string;
    m_parser.http_version = on_request_http_version;
    m_parser.header_done = on_request_header_done;
    m_parser.http_field = on_request_http_field;
    m_parser.data = this;

}
int HttpRequestParser::hasError() {
    return m_error || http_parser_has_error(&m_parser);
}
int HttpRequestParser::finish() {
    return http_parser_finish(&m_parser);
}
size_t HttpRequestParser::execute(char *data, size_t len) {
    size_t offset  = http_parser_execute(&m_parser,data,len,0);
    memmove(data,data + offset,len - offset);
    return offset;
}
int HttpRequestParser::isFinished(){
    return http_parser_is_finished(&m_parser);
}
HttpRequest::ptr HttpRequestParser::getData() {
    return m_data;
}


//response


void on_response_reason_phrase(void *data, const char *at, size_t length) { 
    HttpResponseParser* parser = static_cast<HttpResponseParser*>(data);
    parser->getData()->setReaon(std::string(at,length));
}

void on_response_status_code(void *data, const char *at, size_t length) { 
    HttpResponseParser* parser = static_cast<HttpResponseParser*>(data);
    HttpStatus s = (HttpStatus)atoi(at);
    parser->getData()->setStatus(s);

}
void on_response_chunk_size(void *data, const char *at, size_t length) { 
    //HttpResponseParser* parser = static_cast<HttpResponseParser*>(data);
}
//TODO
void on_response_http_version(void *data, const char *at, size_t length) { 
    HttpResponseParser* parser = static_cast<HttpResponseParser*>(data);
    if(std::string(at,length) == std::string("HTTP/1.1")){
        parser->getData()->setVersion(0x11);
    }else {
        parser->getData()->setVersion(0x10);
    }

}

void on_response_header_done(void *data, const char *at, size_t length) { 
    //HttpResponseParser* parser = static_cast<HttpResponseParser*>(data);
}
void on_response_last_chunk(void *data, const char *at, size_t length) { 
    //HttpResponseParser* parser = static_cast<HttpResponseParser*>(data);
}


void on_response_http_field(void *data, const char *field, size_t flen, const char *value, size_t vlen){
    HttpResponseParser* parser = static_cast<HttpResponseParser*>(data);
    parser->getData()->addHeader(std::string(field,flen), std::string(value,vlen));
}


HttpResponseParser::HttpResponseParser() : m_error(0){
    m_data.reset(new HttpResponse);
    httpclient_parser_init(&m_parser);

    m_parser.http_field = on_response_http_field;
    m_parser.reason_phrase = on_response_reason_phrase;
    m_parser.status_code = on_response_status_code;
    m_parser.chunk_size = on_response_chunk_size;
    m_parser.http_version = on_response_http_version;
    m_parser.header_done = on_response_header_done;
    m_parser.last_chunk = on_response_last_chunk;
    m_parser.data = this;
}

int HttpResponseParser::hasError() {
        return m_error || httpclient_parser_has_error(&m_parser);
}

int HttpResponseParser::finish() {
    return httpclient_parser_finish(&m_parser);
}

int HttpResponseParser::isFinished() {
    return httpclient_parser_is_finished(&m_parser);
}

size_t HttpResponseParser::execute(char *data, size_t len) {
    size_t offset = httpclient_parser_execute(&m_parser,data,len,0);
    memmove(data,data + offset,len - offset);
    return offset;
}

HttpResponse::ptr HttpResponseParser::getData(){
    return m_data;
}

}
}
