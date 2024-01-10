#ifndef DZY_CONFIG_H__
#define DZY_CONFIG_H__

#include <exception>
#include <memory>
#include <functional>
#include <boost/lexical_cast.hpp>
#include <utility>
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

template<class T>
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    ConfigVar(const std::string& name,const T& val,const std::string& discription = ""):ConfigVarBase(name,discription),m_val(val){
    }
 
    std::string toString() override {
        try{
            return boost::lexical_cast<std::string>(m_val);
        }catch(std::exception& e){
                DZY_LOG_ERROR(DZY_LOG_ROOT()) << "e.what()  "<<e.what()<<"ConfigVar bad toString from  " << typeid(m_val).name();
        }
            return "";
    }
    bool fromString(const std::string& str)override {
            try{
                    m_val = boost::lexical_cast<T>(str);
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
private:
    static ConfigMap s_configs;

};






}
#endif

