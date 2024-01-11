#include "config.h"
#include "log.h"
#include <sstream>
#include <utility>
#include <vector>
#include <yaml-cpp/node/detail/iterator_fwd.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>



namespace dzy {
Config::ConfigMap Config::s_configs;
ConfigVarBase::ptr Config::LookupBase(const std::string& name){
        auto it = s_configs.find(name);
        return it == s_configs.end() ? nullptr : it->second;
    

}

void Config::ListAllMember(std::string prefix,const YAML::Node& node, std::list<std::pair<std::string,const YAML::Node> >& output){
        output.push_back(std::make_pair(prefix,node));
        if(node.IsMap()){
            
                for(auto  it = node.begin();it != node.end();it++){
                        ListAllMember(prefix.empty() ? it->first.Scalar() : (prefix + "." + it->first.Scalar()) , it->second , output);
                }
        }

}



void Config::LoadFromYaml(const YAML::Node& node){
            std::list<std::pair<std::string,const YAML::Node> > all_members;
            ListAllMember("", node, all_members);
            for(auto i : all_members){
                auto it = LookupBase(i.first);
                if(it){
                    if(i.second.IsScalar()){
                        it->fromString(i.second.Scalar());
                    }else{
                        std::stringstream os;
                        os << node;
                        it->fromString(os.str());
                    }
                }
            }
}

}
