#include "../dzy/hook.h"
#include "../dzy/iomanager.h"
#include "../dzy/log.h"
#include <arpa/inet.h>
#include "../dzy/log.h"
#include <asm-generic/errno.h>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "../dzy/macro.h"
void test_sleep(){
    dzy::IOManager iom(1);
    iom.schedule([](){
        sleep(5);
        DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "sleep 5";
        });
    iom.schedule([](){
        sleep(10);
        DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "sleep 10";
        });
}
int sock = -1;
void test_io(){
    sock = socket(AF_INET,SOCK_STREAM, 0);
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "sock="<<sock;
   fcntl(sock,F_SETFL,O_NONBLOCK);
    DZY_ASSERT(sock > 0);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);

    if(!connect(sock,(sockaddr*)&addr, sizeof(addr))){
        DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "ret=";
        int ret = write(sock,"A",1);
        DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "ret="<<ret;
    }else if(errno == EINPROGRESS){
   // iom.addEvent(sock,dzy::IOManager::READ,[](){
   //             DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "add read";
   //             dzy::IOManager::GetThis()->cancleEvent(sock, dzy::IOManager::WRITE);
   //     }) ;
       //iom.addEvent(sock,dzy::IOManager::WRITE,[](){
       //         DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "add write";
       // }) ;

    }
    char buf[] = "GET / HTTP/1.1";
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "sizeof buf="<<sizeof(buf);
    
    int ret = write(sock,buf,sizeof(buf));
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "ret="<<ret;

    char bufrcv[1025 * 8];
    memset(bufrcv, 0, sizeof(bufrcv));


    int rt = recv(sock,bufrcv,sizeof(bufrcv),0);
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << bufrcv << "   rt:" <<rt;
}

int main(){
//test_sleep();
    dzy::IOManager iom(3);
    iom.schedule(test_io);
  //   test_io();
return 0;
}
