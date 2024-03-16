#include "../dzy/addr.h"
#include "../dzy/log.h"
#include "../dzy/fiber.h"

//static dzy::Logger::ptr g_logger = DZY_LOG_ROOT();
int main(int argc,char** argv){
    dzy::Fiber::GetThis();
    dzy::IPv4Address::ptr addr = dzy::IPv4Address::Create("127.1.1.1",20);
    std::vector<dzy::IPAddress::ptr> addrsV4;
    std::vector<dzy::IPAddress::ptr> addrsV6;
    dzy::IPAddress::Lookup("www.baidu.com", addrsV4, addrsV6);
    DZY_LOG_INFO(DZY_LOG_ROOT()) << addr->toString();
    for(auto& i : addrsV4){
        DZY_LOG_INFO(DZY_LOG_ROOT()) << i->toString();
    }
    for(auto& i : addrsV6){
        DZY_LOG_INFO(DZY_LOG_ROOT()) << i->toString();
    }

    return 0;
}

