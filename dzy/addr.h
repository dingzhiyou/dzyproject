#pragma once

#include <memory>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

namespace dzy {

class Addreess {
public:
    typedef std::shared_ptr<Addreess> ptr;

    virtual struct sockaddr* getAddr() const = 0;
    virtual socklen_t getAddrLen() const = 0;
    
    std::string toString();
    virtual void insert(std::stringstream& os) = 0;

    bool operator<(const Addreess::ptr r) const;
    bool operator==(const Addreess::ptr r) const;
    bool operator!=(const Addreess::ptr r) const;
};


class IPAddress :public Addreess {
public: 
    typedef std::shared_ptr<IPAddress> ptr;
    
    static void Lookup(const std::string& host,std::vector<IPAddress::ptr>& addrsV4,std::vector<IPAddress::ptr>& addrsV6);
    static IPAddress::ptr Create(const struct sockaddr* addr, uint16_t port = 0);
    virtual uint16_t getPort() const = 0;
    virtual void setPort(uint16_t port) = 0;

private:
};

class IPv4Address : public IPAddress {
public: 
    typedef std::shared_ptr<IPv4Address> ptr;

    IPv4Address(uint32_t addr, uint16_t port = 0);
    IPv4Address();

    static  IPv4Address::ptr Create(const char* addr, uint16_t  port = 0);
    static  IPv4Address::ptr Create(const struct sockaddr_in* addr ,uint16_t port = 0);

    struct sockaddr* getAddr() const  override;
    socklen_t getAddrLen() const override;
    void insert(std::stringstream& os) override;
    uint16_t getPort() const override;
    void setPort(uint16_t port) override;
private:
    struct sockaddr_in m_addr;
};

class IPv6Address : public IPAddress {
public: 
    typedef std::shared_ptr<IPv6Address> ptr;
    IPv6Address();

    static  IPv6Address::ptr Create(const struct sockaddr_in6* addr ,uint16_t = 0);

    struct sockaddr* getAddr()  const override;
    socklen_t getAddrLen() const override;
    void insert(std::stringstream& os) override;
    uint16_t getPort() const override;
    void setPort(uint16_t port) override;
private:
    struct sockaddr_in6 m_addr;
};

class UNKnowAddress : public Addreess {
public:
    typedef std::shared_ptr<UNKnowAddress> ptr;
    UNKnowAddress(struct sockaddr* addr);
    struct sockaddr* getAddr()  const override;
    socklen_t getAddrLen() const override;
    void insert(std::stringstream& os) override;
    
private:
    struct sockaddr m_addr;
};


}
