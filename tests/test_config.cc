#include "../dzy/config.h"
#include <vector>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>



dzy::ConfigVar<int>::ptr g_int = dzy::Config::Lookup("dzy.port", (int)1,"aaa");
dzy::ConfigVar<float>::ptr g_float = dzy::Config::Lookup("dzy.float", (float)1,"aaa");
dzy::ConfigVar<std::vector<int> >::ptr g_vec= dzy::Config::Lookup("dzy.vec",std::vector<int>{1,2},"aaa");
//dzy::ConfigVar<std::vector<int> >::ptr g_vec = dzy::Config::Lookup("dzy-int",std::vector<int>{1,2},"aaa");
dzy::ConfigVar<std::list<int> >::ptr g_lis= dzy::Config::Lookup("dzy.lis",std::list<int>{1,2},"aaa");
void test_yaml(){
    YAML::Node node = YAML::LoadFile("/home/dada/project/bin/conf/log_yaml");
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << node;
}
void test_log(){
    YAML::Node node = YAML::LoadFile("/home/dada/project/bin/conf/log_yaml");
    dzy::Config::LoadFromYaml(node);

}
int main(){
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "int-before : "<<g_int->getVal();
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "float-before: "<<g_float->getVal();
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "vec-before: "<<g_vec->toString();
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "list-before: "<<g_lis->toString();
    test_log();

    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "int-after: "<<g_int->toString();
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "float-after: "<<g_float->getVal();
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "vec-after: "<<g_vec->toString();
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "list-after: "<<g_lis->toString();
    return 0;
}
