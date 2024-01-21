#include "../dzy/thread.h"
#include "../dzy/log.h"
#include <unistd.h>
#include <vector>

int count = 0;
dzy::Mutex mutex;
void fun(){
    for(int i = 0;i < 10;i++){
        dzy::Mutex::LockGuard lock(mutex);
        count++;
        DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "threadid: "<<dzy::Thread::GetThis()->getTid();
    }
}

void test(){
    std::vector<dzy::Thread::ptr> vec;
    for(int i = 0; i< 10; i++){
        vec.push_back(dzy::Thread::ptr(new dzy::Thread(fun)));
    }
    for(int i = 0; i< 10; i++){
        vec[i]->join();
    }
}
int main(){
    test();
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << count;

}
