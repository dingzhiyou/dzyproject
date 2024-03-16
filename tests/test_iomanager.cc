#include "../dzy/iomanager.h"
#include <arpa/inet.h>
#include "../dzy/log.h"
#include <asm-generic/errno.h>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include "../dzy/macro.h"
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <functional>
#include <unistd.h>

void func(){
    DZY_LOG_DEBUG(DZY_LOG_ROOT())<<"hello func";
}

void test(){
    dzy::IOManager iom(3);
    iom.schedule(func);
    iom.schedule(func);
    iom.schedule(func);
    iom.schedule(func);
    iom.schedule(func);
    iom.schedule(func);
}

int sock;
void test_io(){
        dzy::IOManager iom(3);
//        int sock = ::socket(AF_INET,SOCK_STREAM,0);
//        DZY_LOG_INFO(DZY_LOG_ROOT()) << "socket " <<sock;
//        //fcntl(sock, F_SETFL,O_NONBLOCK);
//        sockaddr_in addr;
//        addr.sin_family = AF_INET;
//        addr.sin_port = htons(80);
//        inet_pton(AF_INET,"36.155.132.3",&addr.sin_addr.s_addr); 
//        int ret = connect(sock,(sockaddr*)&addr, sizeof(addr));
//        DZY_LOG_INFO(DZY_LOG_ROOT()) << "ret" <<ret;
//        iom.addEvent(sock,dzy::IOManager::WRITE,[](){
//                DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "connect";
//        }) ;
//
        
    sock = socket(AF_INET,SOCK_STREAM, 0);
    fcntl(sock,F_SETFL,O_NONBLOCK);
    DZY_ASSERT(sock > 0);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET,"36.155.132.3",&addr.sin_addr.s_addr);

    

    if(!connect(sock,(sockaddr*)&addr, sizeof(addr))){
        DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "ret=";
        int ret = write(sock,"A",1);
        DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "ret="<<ret;
    }else if(errno == EINPROGRESS){
    iom.addEvent(sock,dzy::IOManager::READ,[](){
                DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "connect";
                dzy::IOManager::GetThis()->cancleEvent(sock, dzy::IOManager::WRITE);
        }) ;
       iom.addEvent(sock,dzy::IOManager::WRITE,[](){
                DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "connect";
                dzy::IOManager::GetThis()->cancleEvent(sock, dzy::IOManager::READ);
        }) ;


           }else{
    }
   
}
static int i = 0;
dzy::Timer::ptr t;
void timer(){
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "timer call ";
    i++;
    if(i == 3){
        t->cancle();
    }
}

void test_timer(){
    dzy::IOManager iom(3);
    t = iom.addTimer((uint64_t)8000,timer,true);

}

int main(){
    test_io();
 //   test_timer();
    return 0;
}
