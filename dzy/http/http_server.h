#include "../tcp_server.h"
#include "../http/servlet.h"

namespace dzy {
namespace http {

class HttpServer :public TcpServer {
public:
    typedef std::shared_ptr<HttpServer> ptr;
    HttpServer();
    void handleClient(Socket::ptr client) override;
private:
    HttpServletDispatch::ptr  m_dispatch;
};

}
}
