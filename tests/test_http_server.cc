#include "../dzy/http/http_server.h"


void run(){
    dzy::http::HttpServer::ptr  server(new dzy::http::HttpServer);
    dzy::IPv4Address::ptr addr = dzy::IPv4Address::Create("127.0.0.1",8020);
    server->bind(addr);
    server->start();
}

int main(){
    dzy::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
