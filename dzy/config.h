#ifndef DZY_CONFIG_H__
#define DZY_CONFIG_H__

#include <exception>
#include <memory>
#include <functional>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <utility>
#include <vector>
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

template<class T, class F>
class LexicalCast {
public:
        T operator()(const F& v){
            return boost::lexical_cast<T>(v);
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
            std::cout<<ss.str()<<std::endl;
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
            std::cout<<ss.str()<<std::endl;
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
    static typename ConfigVar<T>::ptr  Lookup1(const std::string& name) {
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

