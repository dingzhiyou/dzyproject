#include "../tcp_server.h"

namespace dzy {
namespace http {

class HttpServer :public TcpServer {
public:
    typedef std::shared_ptr<HttpServer> ptr;
    void handleClient(Socket::ptr client) override;
private:
};

}
}
