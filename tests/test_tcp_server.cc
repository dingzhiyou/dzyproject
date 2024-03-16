#include "../dzy/tcp_server.h"
#include  "../dzy/log.h"
#include "../dzy/addr.h"
#include <unistd.h>
static dzy::Logger::ptr g_logger = DZY_LOG_ROOT();

void test_tcp_server(){
    dzy::TcpServer::ptr  server(new dzy::TcpServer);
    auto addr = dzy::IPv4Address::Create("127.0.0.1",8020);
    std::vector<dzy::Addreess::ptr> addrs;
    std::vector<dzy::Addreess::ptr> fails;
    addrs.push_back(addr);
    while(!server->bind(addrs,fails)){
        sleep(2);
    }
    DZY_LOG_INFO(g_logger) <<"===============";
    server->start();

}

int main (){
    dzy::IOManager iom(2);
    iom.schedule(test_tcp_server);
    return 0;
}

