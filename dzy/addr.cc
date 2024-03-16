#include "addr.h"
#include <algorithm>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <string.h>
#include "log.h"
namespace dzy {

static Logger::ptr g_logger = DZY_LOG_ROOT();

bool Addreess::operator<(const Addreess::ptr r) const {
    socklen_t len = std::min(getAddrLen(),r->getAddrLen());
    return memcmp(getAddr(),r->getAddr(),len) < 0 ? true : false;
}
bool Addreess::operator==(const Addreess::ptr r) const{
    socklen_t len = std::min(getAddrLen(),r->getAddrLen());
    return memcmp(getAddr(),r->getAddr(),len) == 0 ? true : false;
}
bool Addreess::operator!=(const Addreess::ptr r) const{
    socklen_t len = std::min(getAddrLen(),r->getAddrLen());
    return memcmp(getAddr(),r->getAddr(),len) == 0 ? false: true;
}
std::string Addreess::toString() {
    std::stringstream os;
    insert(os);
    return os.str();
}
void IPAddress::Lookup(const std::string& host,std::vector<IPAddress::ptr>& addrsV4,std::vector<IPAddress::ptr>& addrsV6){
   size_t colonPos = host.find(':');
    std::string node, service;

    if (colonPos != std::string::npos) {
        // 如果找到冒号，拆分成node和service
        node = host.substr(0, colonPos);
        service = host.substr(colonPos + 1);
    } else {
        // 没有冒号，使用整个字符串作为node
        node = host;
    }

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // 支持 IPv4 和 IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP 套接字

    if (getaddrinfo(node.c_str(), service.empty() ? nullptr : service.c_str(), &hints, &res) != 0) {
        DZY_LOG_ERROR(g_logger)<< "getaddrinfo failed";
        freeaddrinfo(res);
        return;
    }

    // 遍历返回的地址信息链表
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        if (p->ai_family == AF_INET) {
            // IPv4 地址
            struct sockaddr_in *ipv4_addr = reinterpret_cast<struct sockaddr_in*>(p->ai_addr);
            addrsV4.push_back(IPv4Address::Create(ipv4_addr,ipv4_addr->sin_port));
        } else if (p->ai_family == AF_INET6) {
            // IPv6 地址
            struct sockaddr_in6 *ipv6_addr = reinterpret_cast<struct sockaddr_in6*>(p->ai_addr);
            addrsV6.push_back(IPv6Address::Create(ipv6_addr,ipv6_addr->sin6_port));
        }
    }
    // 释放地址信息链表内存
    freeaddrinfo(res);
}

IPAddress::ptr IPAddress::Create(const struct sockaddr* addr, uint16_t port){
    switch (addr->sa_family) {
        case AF_INET: 
                    return IPv4Address::Create((struct sockaddr_in*)addr,port);
            break;
        case AF_INET6:
                    return IPv6Address::Create((struct sockaddr_in6*)addr,port);
            break;
        default:
            return nullptr;
            break;
    }
}

IPv4Address::IPv4Address(uint32_t addr, uint16_t port){
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = addr;
    m_addr.sin_port = htons(port);
}

IPv4Address::IPv4Address(){
    memset(&m_addr,0,sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    DZY_LOG_DEBUG(g_logger) <<"AF_INET="<<AF_INET;

}
IPv4Address::ptr IPv4Address::Create(const char* addr, uint16_t  port){
    IPv4Address::ptr address(new IPv4Address);
    if(int rt = inet_pton(AF_INET,addr,&address->m_addr.sin_addr.s_addr) != 1){
        DZY_LOG_ERROR(g_logger) << "inet_pton error rt="<<rt;
        return nullptr;
    }
    
    address->m_addr.sin_port = htons(port);
    return address;
}
IPv4Address::ptr IPv4Address::Create(const struct sockaddr_in* addr ,uint16_t port){
    IPv4Address::ptr address(new IPv4Address);
    memset(&address->m_addr, 0, sizeof(m_addr));
    address->m_addr = *addr;
    address->m_addr.sin_port = htons(port);
    return address;
}
struct sockaddr* IPv4Address::getAddr() const {
    DZY_LOG_DEBUG(g_logger)<<"==========family="<<m_addr.sin_family;
    return (struct sockaddr*)&m_addr;
}
socklen_t IPv4Address::getAddrLen() const {
    return sizeof(m_addr);
}
void IPv4Address::insert(std::stringstream& os) {
    char buf[1024];
    memset(buf,0, sizeof(buf));
    inet_ntop(AF_INET,&m_addr.sin_addr.s_addr,buf,INET_ADDRSTRLEN);
    os << "AF_INET=" << m_addr.sin_family 
       << "Address=" << buf <<"  port="<<ntohs(m_addr.sin_port);
}
uint16_t IPv4Address::getPort() const {
    return m_addr.sin_port;
}
void IPv4Address::setPort(uint16_t port) {
    m_addr.sin_port = port;
}

IPv6Address::IPv6Address(){
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin6_family = AF_INET6;
}
IPv6Address::ptr IPv6Address::Create(const struct sockaddr_in6* addr ,uint16_t port){
    IPv6Address::ptr address(new IPv6Address);
    memset(&address->m_addr, 0, sizeof(m_addr));
    address->m_addr = *addr;
    address->m_addr.sin6_port = htons(port);
    return address;
}
struct sockaddr* IPv6Address::getAddr() const {
    return (struct sockaddr*)&m_addr;
}
socklen_t IPv6Address::getAddrLen() const {
    return sizeof(m_addr);
}
void IPv6Address::insert(std::stringstream& os) {
    char buf[1024];
    memset(buf,0, sizeof(buf));
    inet_ntop(AF_INET6,&m_addr.sin6_addr,buf,INET6_ADDRSTRLEN);
    os << "AF_INET6=" << m_addr.sin6_family 
       << "Address=" << buf <<"  port="<<m_addr.sin6_port;
}
uint16_t IPv6Address::getPort() const {
    return m_addr.sin6_port;
}
void IPv6Address::setPort(uint16_t port) {
    m_addr.sin6_port = port;
}
UNKnowAddress::UNKnowAddress(struct sockaddr* addr){
    m_addr = *addr;
}
struct sockaddr* UNKnowAddress::getAddr()  const {
    return (struct sockaddr*)&m_addr;
}
socklen_t UNKnowAddress::getAddrLen() const {
    return sizeof(m_addr);
}
void UNKnowAddress::insert(std::stringstream& os) {
    os <<"UNKnowAddress";
}
    


}
