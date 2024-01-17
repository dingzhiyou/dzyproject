#ifndef DZY_CONFIG_H__
#define DZY_CONFIG_H__

#include <exception>
#include <iostream>
#include <memory>
#include <functional>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <utility>
#include <vector>
#include <map>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>
#include "log.h"

namespace dzy {

class ConfigVarBase {
public:
        typedef std::shared_ptr<ConfigVarBase> ptr;
        ConfigVarBase(const std::string& name ,const std::string discription=""):m_name(name),m_discription(discription){

        }
        virtual ~ConfigVarBase() {
        }
        std::string getName()const {return m_name;}
        std::string getDiscription()const {return m_discription;}


        virtual std::string toString() = 0;
        virtual bool fromString(const std::string& str) = 0;
protected:
        std::string m_name;
        std::string m_discription;
        
private:
};


class Person{
public:
        Person(const std::string& name,const int age):m_name(name),m_age(age){}
    std::string m_name;
    int m_age;
};


struct Appender{
 public:
    typedef std::shared_ptr<Appender> ptr;
     int m_type;
     std::string m_filename;

};
struct LogItem {
public:
    typedef std::shared_ptr<LogItem> ptr;
    std::string m_name;
    std::string m_formatter;
    std::string m_level;
    std::vector<Appender::ptr> m_appenders;
};



template<class T, class F>
class LexicalCast {
public:
        T operator()(const F& v){
            return boost::lexical_cast<T>(v);
        }
};


template<>
class LexicalCast<std::vector<LogItem::ptr>, std::string>{
public:
    std::vector<LogItem::ptr> operator()(const std::string& str){
                YAML::Node node =  YAML::Load(str);
                std::vector<LogItem::ptr> vec;
                if(node.IsSequence()){
                    for(size_t s = 0;s < node.size();s++){
                        LogItem::ptr log(new LogItem);
                        log->m_name = node[s]["m_name"].as<std::string>();
                        log->m_formatter = node[s]["m_formatter"].as<std::string>();
                        log->m_level = node[s]["m_level"].as<std::string>();



                        for(size_t i =0 ;i < node[s]["m_appenders"].size();i++){
                        Appender::ptr appen(new Appender); 
                        appen->m_type = node[s]["m_appenders"][i]["m_type"].as<int>();
                        if(appen->m_type == 2){
                            appen->m_filename = node[s]["m_appenders"][i]["m_filename"].as<std::string>();
                        }
                        log->m_appenders.push_back(appen);

                    }
                   vec.push_back(log);
                }
          }
                return vec;
}
};


class Log_init{
public: 
    Log_init();
};

template<>
class LexicalCast<Person,std::string>{
    public:
        Person operator()(const std::string& str){
                YAML::Node node = YAML::Load(str);
                std::string name = node["m_name"].as<std::string>();
                int age = node["m_age"].as<int>();
                Person p(name,age);
                return p;
        }
};
template<>
class LexicalCast<std::string,Person>{
    public:
        std::string operator()(const Person& p){
            YAML::Node node ;
            node["name"] = p.m_name;
            node["age"] = p.m_age;
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
};



template<class T>
class LexicalCast<std::map<std::string,T> ,std::string> { 
public:
    std::map<std::string,T> operator()(const std::string& str) { 
        std::map<std::string,T>  vec;
        YAML::Node node = YAML::Load(str);
        std::stringstream ss;
        for(auto i = node.begin();i != node.end();i++){
            ss.str("");
            ss << i->second;
            vec.insert(std::make_pair(i->first.Scalar(),LexicalCast<T,std::string>()(ss.str())));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::string, std::map<std::string,T> > { 
public:
    std::string operator()(const std::map<std::string,T>& v) { 
        YAML::Node node ;
        for(auto i = v.begin();i != v.end();i++){
                node.push_back(std::make_pair(i->first,LexicalCast<std::string, T>()(i->second)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};



template<class T>
class LexicalCast<std::set<T> ,std::string> { 
public:
    std::set<T> operator()(const std::string& str) { 
        std::set<T>  vec;
        YAML::Node node = YAML::Load(str);
        std::stringstream ss;
        for(size_t i = 0;i < node.size();i++){
            ss.str(""); 
            ss << node[i];
            vec.inset(LexicalCast<T,std::string>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::string, std::set<T> > { 
public:
    std::string operator()(const std::set<T>& v) { 
        YAML::Node node ;
        for(auto i : v){
                node.push_back(LexicalCast<std::string, T>()(i));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};






template<class T>
class LexicalCast<std::vector<T> ,std::string> { 
public:
    std::vector<T> operator()(const std::string& str) { 
        std::vector<T>  vec;
        YAML::Node node = YAML::Load(str);
        std::stringstream ss;
        for(size_t i = 0;i < node.size();i++){
            ss.str(""); 
            ss << node[i];
            vec.push_back(LexicalCast<T,std::string>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::string, std::vector<T> > { 
public:
    std::string operator()(const std::vector<T>& v) { 
        YAML::Node node ;
        for(auto i : v){
                node.push_back(LexicalCast<std::string, T>()(i));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};


template<class T>
class LexicalCast<std::list<T> ,std::string> { 
public:
    std::list<T> operator()(const std::string& str) { 
        std::list<T>  vec;
        YAML::Node node = YAML::Load(str);
        std::stringstream ss;
        for(size_t i = 0;i < node.size();i++){
            ss.str(""); 
            ss << node[i];
            vec.push_back(LexicalCast<T,std::string>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::string, std::list<T> > { 
public:
    std::string operator()(const std::list<T>& v) { 
        YAML::Node node ;
        for(auto i : v){
                node.push_back(LexicalCast<std::string, T>()(i));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};





template<class T ,class FromString = LexicalCast<T, std::string>, class ToString = LexicalCast<std::string, T> >
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    ConfigVar(const std::string& name,const T& val,const std::string& discription = ""):ConfigVarBase(name,discription),m_val(val){
    }
 
    std::string toString() override {
        try{
            return ToString()(m_val);
        }catch(std::exception& e){
                DZY_LOG_ERROR(DZY_LOG_ROOT()) << "e.what()  "<<e.what()<<"ConfigVar bad toString from  " << typeid(m_val).name();
        }
            return "";
    }
    bool fromString(const std::string& str)override {
            try{
                    m_val = FromString()(str);
            }catch(std::exception& e){
                DZY_LOG_ERROR(DZY_LOG_ROOT()) << "e.what()  "<<e.what()<<"ConfigVar bad fromString   " << typeid(m_val).name();
                return false;
        }
            return true;
    }
    const T&  getVal()const {return m_val;}
    void  setVal(const T& val) {m_val = val;}
   
private:
    T m_val;
};

class Config {
public:
    typedef std::shared_ptr<Config> ptr;
    typedef std::map<std::string, ConfigVarBase::ptr> ConfigMap;
    
    template<class T>
    static typename ConfigVar<T>::ptr  Lookup(const std::string& name) {
        auto it = s_configs.find(name);
        if(it == s_configs.end()){
                return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
    }


    template<class T>
    static typename ConfigVar<T>::ptr  Lookup(const std::string& name,const T& default_val,const std::string& discription="") {
        auto it = s_configs.find(name);
        if(it == s_configs.end()){
            ConfigVarBase::ptr config(new ConfigVar<T>(name,default_val,discription));
            s_configs.insert(std::make_pair(name, config));
            return std::dynamic_pointer_cast<ConfigVar<T>>(config);
        }
        return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
   }
  static void LoadFromYaml(const YAML::Node& node);
  static ConfigVarBase::ptr LookupBase(const std::string& name);
  static void ListAllMember(std::string prefix,const YAML::Node& node, std::list<std::pair<std::string,const YAML::Node> >& output);
private:
    static ConfigMap s_configs;
};
}
#endif

