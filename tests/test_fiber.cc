#include "../dzy/fiber.h"
#include "../dzy/log.h"
#include "../dzy/thread.h"
#include <vector>
void test(){
    auto f = dzy::Fiber::GetThis();
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "in test";
    dzy::Fiber::YieldToReady();
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "prepare to out test";
}
void test1(){
    dzy::Fiber::GetThis();
    dzy::Fiber::ptr fiber(new dzy::Fiber(test));
    fiber->swapIn();
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "prepare to in test";
    fiber->swapIn();

    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "fin out test";

}
int main(){
    std::vector<dzy::Thread::ptr> thrs;
    for(int i= 0;i<10;i++){
        thrs.push_back(dzy::Thread::ptr(new dzy::Thread(test1)));
    }

}
