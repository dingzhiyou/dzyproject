#include "../tcp_server.h"
#include <memory>

namespace dzy {
namespace ftp {

class FtpServer : public TcpServer {
public:
        typedef std::shared_ptr<FtpServer> ptr;
        void handleClient(Socket::ptr sock) override;

private:
};

}
}
