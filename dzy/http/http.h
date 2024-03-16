#pragma once

#include <bits/stdint-uintn.h>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <map>


namespace dzy{
namespace http{

#define HTTP_STATUS_MAP(XX)                                                 \
  XX(100, CONTINUE,                        Continue)                        \
  XX(101, SWITCHING_PROTOCOLS,             Switching Protocols)             \
  XX(102, PROCESSING,                      Processing)                      \
  XX(200, OK,                              OK)                              \
  XX(201, CREATED,                         Created)                         \
  XX(202, ACCEPTED,                        Accepted)                        \
  XX(203, NON_AUTHORITATIVE_INFORMATION,   Non-Authoritative Information)   \
  XX(204, NO_CONTENT,                      No Content)                      \
  XX(205, RESET_CONTENT,                   Reset Content)                   \
  XX(206, PARTIAL_CONTENT,                 Partial Content)                 \
  XX(207, MULTI_STATUS,                    Multi-Status)                    \
  XX(208, ALREADY_REPORTED,                Already Reported)                \
  XX(226, IM_USED,                         IM Used)                         \
  XX(300, MULTIPLE_CHOICES,                Multiple Choices)                \
  XX(301, MOVED_PERMANENTLY,               Moved Permanently)               \
  XX(302, FOUND,                           Found)                           \
  XX(303, SEE_OTHER,                       See Other)                       \
  XX(304, NOT_MODIFIED,                    Not Modified)                    \
  XX(305, USE_PROXY,                       Use Proxy)                       \
  XX(307, TEMPORARY_REDIRECT,              Temporary Redirect)              \
  XX(308, PERMANENT_REDIRECT,              Permanent Redirect)              \
  XX(400, BAD_REQUEST,                     Bad Request)                     \
  XX(401, UNAUTHORIZED,                    Unauthorized)                    \
  XX(402, PAYMENT_REQUIRED,                Payment Required)                \
  XX(403, FORBIDDEN,                       Forbidden)                       \
  XX(404, NOT_FOUND,                       Not Found)                       \
  XX(405, METHOD_NOT_ALLOWED,              Method Not Allowed)              \
  XX(406, NOT_ACCEPTABLE,                  Not Acceptable)                  \
  XX(407, PROXY_AUTHENTICATION_REQUIRED,   Proxy Authentication Required)   \
  XX(408, REQUEST_TIMEOUT,                 Request Timeout)                 \
  XX(409, CONFLICT,                        Conflict)                        \
  XX(410, GONE,                            Gone)                            \
  XX(411, LENGTH_REQUIRED,                 Length Required)                 \
  XX(412, PRECONDITION_FAILED,             Precondition Failed)             \
  XX(413, PAYLOAD_TOO_LARGE,               Payload Too Large)               \
  XX(414, URI_TOO_LONG,                    URI Too Long)                    \
  XX(415, UNSUPPORTED_MEDIA_TYPE,          Unsupported Media Type)          \
  XX(416, RANGE_NOT_SATISFIABLE,           Range Not Satisfiable)           \
  XX(417, EXPECTATION_FAILED,              Expectation Failed)              \
  XX(421, MISDIRECTED_REQUEST,             Misdirected Request)             \
  XX(422, UNPROCESSABLE_ENTITY,            Unprocessable Entity)            \
  XX(423, LOCKED,                          Locked)                          \
  XX(424, FAILED_DEPENDENCY,               Failed Dependency)               \
  XX(426, UPGRADE_REQUIRED,                Upgrade Required)                \
  XX(428, PRECONDITION_REQUIRED,           Precondition Required)           \
  XX(429, TOO_MANY_REQUESTS,               Too Many Requests)               \
  XX(431, REQUEST_HEADER_FIELDS_TOO_LARGE, Request Header Fields Too Large) \
  XX(451, UNAVAILABLE_FOR_LEGAL_REASONS,   Unavailable For Legal Reasons)   \
  XX(500, INTERNAL_SERVER_ERROR,           Internal Server Error)           \
  XX(501, NOT_IMPLEMENTED,                 Not Implemented)                 \
  XX(502, BAD_GATEWAY,                     Bad Gateway)                     \
  XX(503, SERVICE_UNAVAILABLE,             Service Unavailable)             \
  XX(504, GATEWAY_TIMEOUT,                 Gateway Timeout)                 \
  XX(505, HTTP_VERSION_NOT_SUPPORTED,      HTTP Version Not Supported)      \
  XX(506, VARIANT_ALSO_NEGOTIATES,         Variant Also Negotiates)         \
  XX(507, INSUFFICIENT_STORAGE,            Insufficient Storage)            \
  XX(508, LOOP_DETECTED,                   Loop Detected)                   \
  XX(510, NOT_EXTENDED,                    Not Extended)                    \
  XX(511, NETWORK_AUTHENTICATION_REQUIRED, Network Authentication Required) \



/* Request Methods */
#define HTTP_METHOD_MAP(XX)         \
  XX(0,  DELETE,      DELETE)       \
  XX(1,  GET,         GET)          \
  XX(2,  HEAD,        HEAD)         \
  XX(3,  POST,        POST)         \
  XX(4,  PUT,         PUT)          \
  /* pathological */                \
  XX(5,  CONNECT,     CONNECT)      \
  XX(6,  OPTIONS,     OPTIONS)      \
  XX(7,  TRACE,       TRACE)        \
  /* WebDAV */                      \
  XX(8,  COPY,        COPY)         \
  XX(9,  LOCK,        LOCK)         \
  XX(10, MKCOL,       MKCOL)        \
  XX(11, MOVE,        MOVE)         \
  XX(12, PROPFIND,    PROPFIND)     \
  XX(13, PROPPATCH,   PROPPATCH)    \
  XX(14, SEARCH,      SEARCH)       \
  XX(15, UNLOCK,      UNLOCK)       \
  XX(16, BIND,        BIND)         \
  XX(17, REBIND,      REBIND)       \
  XX(18, UNBIND,      UNBIND)       \
  XX(19, ACL,         ACL)          \
  /* subversion */                  \
  XX(20, REPORT,      REPORT)       \
  XX(21, MKACTIVITY,  MKACTIVITY)   \
  XX(22, CHECKOUT,    CHECKOUT)     \
  XX(23, MERGE,       MERGE)        \
  /* upnp */                        \
  XX(24, MSEARCH,     M-SEARCH)     \
  XX(25, NOTIFY,      NOTIFY)       \
  XX(26, SUBSCRIBE,   SUBSCRIBE)    \
  XX(27, UNSUBSCRIBE, UNSUBSCRIBE)  \
  /* RFC-5789 */                    \
  XX(28, PATCH,       PATCH)        \
  XX(29, PURGE,       PURGE)        \
  /* CalDAV */                      \
  XX(30, MKCALENDAR,  MKCALENDAR)   \
  /* RFC-2068, section 19.6.1.2 */  \
  XX(31, LINK,        LINK)         \
  XX(32, UNLINK,      UNLINK)       \
  /* icecast */                     \
  XX(33, SOURCE,      SOURCE)       \


enum class HttpMethod{
#define XX(num,name,string)  name = num ,
    HTTP_METHOD_MAP(XX)
#undef XX
    INVILD_METHOD
};

enum class HttpStatus{
#define XX(code,name,discription)  name = code,
    HTTP_STATUS_MAP(XX)
#undef XX
};

HttpMethod StringToHttpMethod(std::string& m);
HttpMethod CharsToHttpMethod(const char* m);
std::string HttpMethodToString(HttpMethod& h);
std::string HttpStatusToString(HttpStatus& h);


class CaseInSensitiveCompare {
public:
    bool operator()(std::string lstr,std::string rstr);
};

class HttpRequest {
public:
    typedef std::shared_ptr<HttpRequest> ptr;
    typedef std::map<std::string,std::string,CaseInSensitiveCompare> MapType;

    HttpRequest(uint8_t version = 0x11, bool close = true);
    uint8_t getVersion() const { return m_version;}
    HttpMethod getMethod()const {return m_method;}
    void setMethod(const HttpMethod&  m) {m_method = m;}


    const std::string getPath() const {return m_path;}
    const std::string getQuery()const  {return m_query;}
    const std::string getFragment() const  {return m_fragment;}


    void setPath(std::string v) { m_path = v;}
    void setQuery(std::string v) { m_query = v;}
    void setFragment(std::string v) { m_fragment = v;}

    const std::string getBody()const {return m_body;}
    void setBody(std::string& b) {m_body = b;}

    const MapType getHeaders() const { return m_headers;}
    const MapType getParams() const { return m_params;}
    const MapType getCookies() const { return m_cookies;}

    uint8_t getClose() const {return m_version;}
    void setClose(const uint8_t close) {m_close = close;}

    void setHeaders(MapType& v) { m_headers = v;}
    void setParams(MapType& v) { m_params = v;}
    void setCookies(MapType& v) { m_cookies = v;}

    void addHeader(std::string key,std::string val);
    void addParam(std::string key,std::string val);
    void addCookie(std::string key,std::string val);

    void setVersion(uint8_t version) {m_version = version;}
    void delHeader(std::string key);
    void delParam(std::string key);
    void delCookie(std::string key);


    void getHeader(std::string key,std::string* val = nullptr);
    void getParam(std::string key,std::string* val = nullptr);
    void getCookie(std::string key,std::string* val = nullptr);

    std::string toString();
    void dump(std::stringstream& os);
private:
    HttpMethod m_method;
    uint8_t m_version;
    bool m_close;

    std::string m_path;
    std::string m_query;
    std::string m_fragment;

    std::string m_body;

    MapType m_headers;
    MapType m_params;
    MapType m_cookies;
};


class HttpResponse {
public:
    typedef std::shared_ptr<HttpResponse> ptr;
    typedef std::map<std::string,std::string,CaseInSensitiveCompare> MapType;

    HttpResponse(uint8_t v =0x11,HttpStatus s = HttpStatus::OK,bool close = false);
    HttpStatus getStatus() const {return  m_status;}
    std::string getReason() const {return m_reason;}
    uint8_t getVersion()const {return m_version;}
    bool getClose()const {return m_close;}
    const  std::string& getBody()const {return m_body;}
    

    const MapType& getHeaders() const {return m_headers;}


    void setStatus(HttpStatus s) {m_status = s;}
    void setReaon(std::string reasons) {m_reason = reasons;}
    void setVersion(uint8_t version) {m_version = version;}

    void setClose(const bool cloes) {m_close = cloes;}
    void setBody(const std::string& body) {m_body = body;}
    void setHeaders(const MapType& m) {m_headers = m;}

    
    void addHeader(std::string key,std::string val);
    void getHeader(std::string key,std::string* val = nullptr);
    void delHeader(std::string key);

    std::string toString();
    void dump(std::stringstream& os);

private:
    uint8_t m_version;
    HttpStatus m_status;
    bool m_close;
    std::string m_reason;

    std::string m_body;
    
    MapType m_headers;

};

std::ostream& operator<<(std::ostream& os,HttpRequest::ptr req);
std::ostream& operator<<(std::ostream& os,HttpResponse::ptr rsp);

}
}

