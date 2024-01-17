#include "../dzy/thread.h"
#include <iostream>
#include <unistd.h>
void func(){
    std::cout<<"thread begin"<<dzy::Thread::GetThis()->getName()<<"\n"<<dzy::Thread::GetThis()->getTid()<<std::endl;
    sleep(100);
}


int main(){
    dzy::Thread::ptr thread(new dzy::Thread(func,"root"));
    thread->join();
    return 0;
}
