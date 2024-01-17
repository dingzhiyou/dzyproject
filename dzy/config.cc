#include "config.h"
#include "log.h"
#include <iostream>
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
                        os << i.second;
                        it->fromString(os.str());
                    }
                }
            }
}


Log_init::Log_init(){
    ConfigVar<std::vector<LogItem::ptr> >::ptr g_logs = Config::Lookup("logs",std::vector<LogItem::ptr>{},"logs");
    YAML::Node node = YAML::LoadFile("/home/dada/project/bin/conf/log_config");
    Config::LoadFromYaml(node);

    auto& logs = g_logs->getVal();
    for(auto i : logs){
       Logger::ptr logger(new Logger(i->m_name));
       logger->setLevel(LogLevel::ToLevel((i->m_level)));
       logger->setFormatter(i->m_formatter);

       for(size_t j = 0;j < i->m_appenders.size();j++){
               if(i->m_appenders[j]->m_type == 1){
                       logger->addAppener(LoggerAppender::ptr(new StdoutAppender));
               }else if(i->m_appenders[j]->m_type == 2){
                       logger->addAppener(LoggerAppender::ptr(new FileAppender(LogLevel::ToLevel(i->m_level),i->m_appenders[j]->m_filename)));
               }else{
               }
       }
       LoggerMgr::GetInstance()->addLogger(i->m_name, logger);
    }

}


Log_init log_init;

}
