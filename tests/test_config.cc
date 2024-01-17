#include "../dzy/config.h"
#include <vector>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>



dzy::ConfigVar<std::vector<dzy::Person> >::ptr g_person= dzy::Config::Lookup("dzy.person",std::vector<dzy::Person>{dzy::Person("dzy",23),dzy::Person("dadad",29)},"aaa");
dzy::ConfigVar<std::map<std::string,int> >::ptr g_map= dzy::Config::Lookup("dzy.dzymap", std::map<std::string,int>{std::make_pair("aaa",1),std::make_pair("bbb",2)},"aaa");
//dzy::ConfigVar<int>::ptr g_int = dzy::Config::Lookup("dzy.port", (int)1,"aaa");
//dzy::ConfigVar<float>::ptr g_float = dzy::Config::Lookup("dzy.float", (float)1,"aaa");
//dzy::ConfigVar<std::vector<int> >::ptr g_vec= dzy::Config::Lookup("dzy.vec",std::vector<int>{1,2},"aaa");
////dzy::ConfigVar<std::vector<int> >::ptr g_vec = dzy::Config::Lookup("dzy-int",std::vector<int>{1,2},"aaa");
//dzy::ConfigVar<std::list<int> >::ptr g_lis= dzy::Config::Lookup("dzy.lis",std::list<int>{1,2},"aaa");
void test_yaml(){
    YAML::Node node = YAML::LoadFile("/home/dada/project/bin/conf/log_yaml");
    DZY_LOG_DEBUG(DZY_LOG_ROOT()) << node;
}
void test_log(){
    YAML::Node node = YAML::LoadFile("/home/dada/project/bin/conf/log_yaml");
    dzy::Config::LoadFromYaml(node);

}
void test1(){
    //DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "int-before : "<<g_int->getVal();
    //DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "float-before: "<<g_float->getVal();
    //DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "vec-before: "<<g_vec->toString();
    //DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "list-before: "<<g_lis->toString();
    auto& map = g_map->getVal();
    for(auto i :map){
         DZY_LOG_DEBUG(DZY_LOG_ROOT()) <<"before-" <<i.first<< "---"<<i.second;
    }
    test_log();
   // DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "int-after: "<<g_int->toString();
   // DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "float-after: "<<g_float->getVal();
   // DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "vec-after: "<<g_vec->toString();
   // DZY_LOG_DEBUG(DZY_LOG_ROOT()) << "list-after: "<<g_lis->toString();
    for(auto i :map){
         DZY_LOG_DEBUG(DZY_LOG_ROOT()) <<"after-" <<i.first<< "---"<<i.second;
    }


}
void test_conf(){
    auto i = g_person->getVal();
    for(auto it : i){
        DZY_LOG_DEBUG(DZY_LOG_ROOT()) << it.m_name<<"---"<<it.m_age ;
    }
}


int main(){
   auto i = g_person->getVal();
    for(auto it : i){
        DZY_LOG_DEBUG(DZY_LOG_ROOT()) << it.m_name<<"---"<<it.m_age ;}
    test_log();
    test_conf();

    return 0;
}
