#include "../dzy/config.h"
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>



dzy::ConfigVar<int>::ptr g_int = dzy::Config::Lookup("dzy-int", (int)1,"aaa");
//dzy::ConfigVar<std::vector<int> >::ptr g_vec = dzy::Config::Lookup("dzy-int",std::vector<int>{1,2},"aaa");

void test_yaml(){
    YAML::Node node = YAML::LoadFile("/home/dada/project/bin/conf/log_yaml");
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << node;
}
int main(){
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << g_int->getVal();
    test_yaml();
    return 0;
}
