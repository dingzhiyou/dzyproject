#include "../dzy/util.h"
#include "../dzy/log.h"


void func(){

    DZY_LOG_INFO(DZY_LOG_ROOT()) << dzy::BacktraceTostring(100);
    DZY_ASSERT2(0 == 1,"abcdefg");
}
void test(){

    func();
}

int main(){
    test();
    return 0;

}
