#include  "../dzy/tcp_server.h"
#include "../dzy/bytearray.h"
#include <cstring>
#include <memory>

static dzy::Logger::ptr g_logger = DZY_LOG_ROOT();

class EchoServer : public dzy::TcpServer {
public:
    typedef std::shared_ptr<EchoServer> ptr;
    EchoServer() {
        DZY_LOG_INFO(g_logger) << "i am  in EchoServer";
    }
    ~EchoServer(){
        DZY_LOG_INFO(g_logger) << "i am  in ~EchoServer";
    }
    void handleClient(dzy::Socket::ptr sock) override;
};


void EchoServer::handleClient(dzy::Socket::ptr sock) {
   DZY_LOG_INFO(g_logger) << "OK";
   dzy::Bytearray::ptr ba(new dzy::Bytearray(5)); 
   char buf[1024 * 4];
   memset(buf, 0, sizeof(buf));
   while(true){
       DZY_LOG_INFO(g_logger) << "OK";
       char buf1[]="dzy";
       sock->send(buf1,sizeof(buf1),0);
       DZY_LOG_INFO(g_logger) << "OK";
       int rt = sock->recv(buf,sizeof(buf),0);
       if(rt == 0){
           DZY_LOG_INFO(g_logger) <<"remote disconnect";
           return;
       }else if(rt > 0){
          ba->write(buf,rt);
          DZY_LOG_INFO(g_logger) <<std::string(buf,rt);
       }else{
           DZY_LOG_INFO(g_logger) <<"error";
           return;
       }
   }
   ba->writeTofile("./echodata",0);
}

void test_tcp_server(){
    EchoServer::ptr  server(new EchoServer);
    DZY_LOG_INFO(g_logger) <<"server.get()="<<server.get();
    DZY_LOG_INFO(g_logger) <<"server" << &server;
    auto addr = dzy::IPv4Address::Create("127.0.0.1",8020);
    std::vector<dzy::Addreess::ptr> addrs;
    std::vector<dzy::Addreess::ptr> fails;
    addrs.push_back(addr);
    while(!server->bind(addrs,fails)){
        sleep(2);
    }
    server->start();
}

int main(){
    dzy::IOManager iom(4);
    iom.schedule(test_tcp_server);
    return 0;
}



