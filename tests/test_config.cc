#include "../dzy/config.h"


dzy::ConfigVar<int>::ptr g_int = dzy::Config::Lookup("dzy-int", (int)1,"aaa");

int main(){
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << g_int->getVal();
    return 0;
}
