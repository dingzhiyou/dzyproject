#include "../dzy/http/http_parser.h"
#include "../dzy/log.h"

static dzy::Logger::ptr g_logger = DZY_LOG_ROOT();
const char data[] = "GET /api/data HTTP/1.0\r\n"
                    "Host: example.com\r\n"
                    "Content-Length: 14\r\n\r\n"
                    "Hello, server!";

const char data2[] = "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: 14\r\n\r\n"
                    "Hello, client!\r\n";


void test_http_request(){
    dzy::http::HttpRequestParser parser;
    std::string tmp = data;
    size_t s = parser.execute(&tmp[0], tmp.size());

    std::string length;
    parser.getData()->getHeader("content-length",&length);
    DZY_LOG_INFO(g_logger) << "Content-Length="<< length 
                           <<"  s="<<s
                           <<" hasError="<<parser.hasError()<<"   data="
                           << parser.getData()->toString();
}
void test_http_response(){
    dzy::http::HttpResponseParser parser;
    std::string tmp =data2;
    size_t s = parser.execute(&tmp[0],tmp.size());
    std::string length;

    parser.getData()->getHeader("content-length",&length);
    DZY_LOG_INFO(g_logger) << "Content-length=" << length
                           <<"  s="<<s
                           <<" hasError="<<parser.hasError()
                           <<"data="<<parser.getData()->toString();
    tmp.resize(tmp.size() - s);
    DZY_LOG_INFO(g_logger) <<"tmp="<<tmp;
}
int main() {
    //test_http_request();
    test_http_response();
    return 0;
}
