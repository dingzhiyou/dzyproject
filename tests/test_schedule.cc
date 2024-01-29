#include "../dzy/schedule.h"
#include "../dzy/log.h"
#include "../dzy/fiber.h"
#include <iostream>
#include <unistd.h>

void func1(){
    DZY_LOG_DEBUG(DZY_LOG_ROOT())<<"hello dzy-------";
}
void func(){
    DZY_LOG_DEBUG(DZY_LOG_ROOT())<<"hello dzy";
    dzy::Schedule::GetThis()->schedule(func);
}

int main(){

    DZY_LOG_DEBUG(DZY_LOG_ROOT()) <<"ttid:"<< dzy::Gettid();
    dzy::Schedule::ptr schedule(new dzy::Schedule(3,true));
    schedule->start();
    schedule->schedule(dzy::Fiber::ptr(new dzy::Fiber(func)));
    schedule->schedule(dzy::Fiber::ptr(new dzy::Fiber(func)));
    schedule->schedule(dzy::Fiber::ptr(new dzy::Fiber(func)));
    schedule->schedule(dzy::Fiber::ptr(new dzy::Fiber(func)));
    schedule->schedule(dzy::Fiber::ptr(new dzy::Fiber(func)));
    schedule->schedule(dzy::Fiber::ptr(new dzy::Fiber(func)));
    schedule->stop();
}
