#pragma once

#include <memory>
#include <functional>
#include <vector>
#include "socket.h"
#include "iomanager.h"
#include "addr.h"

namespace dzy {

class TcpServer : public std::enable_shared_from_this<TcpServer> {
public:
    typedef std::shared_ptr<TcpServer> ptr;
    TcpServer(IOManager* work = IOManager::GetThis(),IOManager* acceptWork = IOManager::GetThis(),std::string name = "TcpServer");
    virtual ~TcpServer();
    std::string getName()const { return m_name;}
    void setName(const std::string name) { m_name = name;}


    uint64_t getRecvTimeout() const {return m_recvTimeout;}
    void setRecvTimeout(uint64_t v) {m_recvTimeout = v;}


    virtual bool bind(Addreess::ptr addr);
    virtual bool bind(std::vector<Addreess::ptr>& addrs,std::vector<Addreess::ptr>& fails);
    virtual bool stop();
    virtual bool start();
    virtual void startAccept(Socket::ptr sock);

    virtual void handleClient(Socket::ptr sock);
protected:
private:
    std::vector<Socket::ptr> m_socks;
    IOManager* m_work;
    IOManager* m_acceptWork;

    std::string m_name;
    bool m_isStop;
    uint64_t m_recvTimeout;

};



}
