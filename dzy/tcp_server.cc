#include "tcp_server.h"
#include "addr.h"
#include "log.h"
#include "socket.h"
#include "bytearray.h"
#include <iostream>

static dzy::Logger::ptr g_logger = DZY_LOG_ROOT();

namespace dzy {
TcpServer::TcpServer(IOManager* work ,IOManager* acceptWork ,std::string name ):m_work(work)
                                                                            ,m_acceptWork(acceptWork)
                                                                            ,m_name(name)
                                                                            ,m_isStop(true)
                                                                            ,m_recvTimeout(60 * 1000 * 2){

    DZY_LOG_INFO(g_logger) << "i am  in TcpServer";
}

TcpServer::~TcpServer(){
    DZY_LOG_INFO(g_logger) << "i am  in ~TcpServer";
    for(auto& sock : m_socks){
        sock->close();
    }
    m_socks.clear();
}
bool TcpServer::bind(Addreess::ptr addr){
    std::vector<Addreess::ptr> addrs;
    std::vector<Addreess::ptr> fails;
    
    addrs.push_back(addr);
    return  bind(addrs,fails);
}
bool TcpServer::bind(std::vector<Addreess::ptr>& addrs,std::vector<Addreess::ptr>& fails) {
    for(auto& addr : addrs) {
        Socket::ptr sock = Socket::CreateTCP();
        if(sock->bind(addr)){
            DZY_LOG_ERROR(g_logger) << "bind error,errno=" << errno << "  errstr=" << strerror(errno)
                                    << " addr=" <<addr->toString();
            fails.push_back(addr);
            return false;
        }
        if(sock->listen()){
            DZY_LOG_ERROR(g_logger) << "listen error,errno=" << errno << "  errstr=" << strerror(errno)
                                    << " addr=" <<addr->toString();
            fails.push_back(addr);
            return false;
        }
        DZY_LOG_INFO(g_logger) << "bind success addr="<<addr->toString();
        sock->setRecvTimeout(m_recvTimeout);
        m_socks.push_back(sock);
    }
    return true;
}
bool TcpServer::stop() {
    m_isStop = true;
    for(auto& sock : m_socks){
        sock->close();
    }
    m_socks.clear();
    return true;
}
bool TcpServer::start() {
    m_isStop = false;
    if(m_isStop){
        return false;
    }
    for(auto& sock : m_socks){
        m_acceptWork->schedule(std::bind(&TcpServer::startAccept,shared_from_this(),sock));
    }
    return true;

}
void TcpServer::startAccept(Socket::ptr sock) {
    while(!m_isStop){
        Socket::ptr client = sock->accept();
        DZY_LOG_INFO(g_logger) <<"after accept";
        if(!client){
            DZY_LOG_INFO(g_logger) << "timeout ";
            continue;
        }
        client->setRecvTimeout(m_recvTimeout);
        DZY_LOG_INFO(g_logger) <<"++++++++++++++++++++++";
        m_work->schedule(std::bind(&TcpServer::handleClient,shared_from_this(),client));
   }

}

//void TcpServer::handleClient(dzy::Socket::ptr sock) {
//    std::cout<<"OK"<<std::endl;
//    dzy::Bytearray::ptr ba(new dzy::Bytearray(5)); 
//    char buf[1024 * 1024 * 4];
//    memset(buf, 0, sizeof(buf));
//   while(true){
//       int rt = sock->recv(buf,sizeof(buf),0);
//       if(rt == 0){
//           DZY_LOG_INFO(g_logger) <<"remote disconnect";
//           return;
//       }else if(rt > 0){
//          ba->write(buf,rt);
//          DZY_LOG_INFO(g_logger) <<std::string(buf,rt);
//       }else{
//           DZY_LOG_INFO(g_logger) <<"error";
//           return;
//       }
//   }
//   ba->writeTofile("./echodata",0);
//}


void TcpServer::handleClient(Socket::ptr sock) {
            char buf[] = "hello dzy";
            sock->send(buf,sizeof(buf),0);
}

}
