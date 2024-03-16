#include "bytearray.h"
#include "log.h"
#include "macro.h"
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <cerrno>
#include <ios>
#include <string.h>
#include <fstream>

namespace dzy{
static uint64_t s_nodeId = 0;
Bytearray::Node::Node(size_t size):next(nullptr)
                                  ,size(size){
        ptr = new char[size];
        id = ++s_nodeId;
}
Bytearray::Node::Node():next(nullptr)
                       ,ptr(nullptr)
                       ,size(0){
}
Bytearray::Bytearray(size_t basesize):m_position(0)
                                     ,m_capacity(basesize)
                                     ,m_size(0)
                                     ,m_basesize(basesize){
        m_root = new Node(m_basesize);
        m_cur = m_root;

}
Bytearray::~Bytearray(){
    Node* tmp = m_root;
    while(tmp){
        m_cur = tmp->next;
        delete[] tmp->ptr;
        tmp = m_cur;
    }
}
//write
void Bytearray::writeFint8(int8_t val){
    int8_t v = val;
    write(&v,sizeof(v));
}
void Bytearray::writeFuint8(uint8_t val){
    uint8_t v= val;
    write(&v,sizeof(v));

}
void Bytearray::writeFint16(int16_t val){
    int16_t v= val;
    write(&v,sizeof(v));
}
void Bytearray::writeFuint16(uint16_t val){
    uint16_t v = val;
    write(&v,sizeof(v));

}
void Bytearray::writeFint32(int32_t val){

    int32_t v = val;
    write(&v,sizeof(v));
}
void Bytearray::writeFuint32(uint32_t val){
    uint32_t v = val;
    write(&v,sizeof(v));
}
void Bytearray::writeFint64(int64_t val){
    int64_t v = val;
    write(&v,sizeof(v));
}
void Bytearray::writeFuint64(uint64_t val){
    uint64_t v= val;
    write(&v,sizeof(v));
}

void Bytearray::writeFloat(float val){
    int32_t v;
    memcpy(&v,&val,sizeof(v));
    write(&v,sizeof(v));
}
void Bytearray::writeDouble(double val){
    int64_t v;
    memcpy(&v,&val,sizeof(v));
    write(&v,sizeof(v));
}

void Bytearray::writeStringF16(std::string& val){
    uint16_t size = val.size();
    writeFuint16(size);
    write(&val[0],val.size());
}
void Bytearray::writeStringF32(std::string& val){
    uint32_t size = val.size();
    writeFuint32(size);
    write(&val[0],val.size());
}
void Bytearray::writeStringF64(std::string& val){
    uint64_t size = val.size();
    writeFuint64(size);
    write(&val[0],val.size());
}
void Bytearray::writeStringVar(std::string& val){
     uint64_t size = val.size();
    writeFuint64(size);
    write(&val[0],val.size());
}

void Bytearray::writeUint32(uint32_t val){
    uint8_t tmp[5];
    int i;
    while(val >= 0x80){
        tmp[i++] = (val & 0x7F) | 0x80;
        val >>= 7;
    }
    tmp[i++] = val;
    write(&tmp[0],i);
}
void Bytearray::writeUint64(uint64_t val){
     uint8_t tmp[10];
    int i;
    while(val >= 0x80){
        tmp[i++] = (val & 0x7F) | 0x80;
        val >>= 7;
    }
    tmp[i++] = val;
    write(&tmp[0],i);
}

//read
#define XX(type) \
    type value; \
    read(&value,sizeof(type)); \
    return value;


int8_t   Bytearray::readFint8(){
    XX(int8_t);   
}
uint8_t  Bytearray::readFuint8(){
    XX(uint8_t);
}
int16_t  Bytearray::readFint16(){
    XX(int16_t);
}
uint16_t Bytearray::readFuint16(){
    XX(uint16_t);
}
int32_t  Bytearray::readFint32(){
    XX(int32_t);
}
uint32_t Bytearray::readFuint32(){
    XX(uint32_t);
}
int64_t  Bytearray::readFint64(){
    XX(int64_t);
}
uint64_t Bytearray::readFuint64(){
    XX(uint64_t);
}
#undef XX
float    Bytearray::readFloat(){
        int32_t value;
        float v;
        value = readFint32();
        memcpy(&v,&value,sizeof(int32_t));
        return v;
}
double   Bytearray::readDouble(){
        int64_t value;
        double v;
        value = readFint64();
        memcpy(&v,&value,sizeof(int64_t));
        return v;
}

std::string Bytearray::readStringF16(){
        uint16_t len = readFuint16();
        std::string str;
        str.resize(len);
        read(&str[0],str.size());
        return str;
}
std::string Bytearray::readStringF32(){
        uint32_t len = readFuint32();
        std::string str;
        str.resize(len);
        read(&str[0],str.size());
        return str;
}
std::string Bytearray::readStringF64(){
        uint64_t len = readFuint64();
        std::string str;
        str.resize(len);
        read(&str[0],str.size());
        return str;
}
std::string Bytearray::readStringVar(){
         uint64_t len = readFuint64();
        std::string str;
        str.resize(len);
        read(&str[0],str.size());
        return str;      
}

uint32_t  Bytearray::readUint32(){
    uint32_t value = 0;
    for(int i = 0; i < 32;i += 7){
        uint8_t b = readFuint8();
        if(b < 0x80){
            value |= (uint32_t)b << i;
            break;
        }else {
            value |= ((uint32_t)(b & 0x7F)) << i;
        }
    }
    return value;

}
uint64_t  Bytearray::readUint64(){
    uint64_t value = 0;
    for(int i = 0; i < 64;i += 7){
        uint8_t b = readFuint8();
        if(b < 0x80){
            value |= (uint64_t)b << i;
            break;
        }else {
            value |= ((uint64_t)(b & 0x7F)) << i;
        }
    }
    return value;
}

static Logger::ptr g_logger = DZY_LOG_ROOT();
void Bytearray::write(void* buf,size_t len){
    if(len == 0){
        return;
    }
    addCapacity(len);
    
    size_t npos = m_position % m_basesize;
    size_t ncap = m_cur->size - npos;
    size_t bpos = 0;
    while(len){
        if(ncap >= len){
            memcpy(m_cur->ptr + npos,(char*)buf + bpos , len);
            m_position += len;
            len  = 0;
            break;
        }else{
            memcpy(m_cur->ptr + npos,(char*)buf + bpos,ncap);
            bpos += ncap;
            len -= ncap;
            m_position += ncap;
            m_cur = m_cur->next;
            ncap = m_cur->size;
            npos = 0;
        }
    }
    if(m_position > m_size){
        m_size = m_position;
    }
}
void Bytearray::read(void* buf,size_t len){
    if(len <= 0){
        return;
    }
    if(getReadsize() < len){
        return;
    }
    
    size_t npos = m_position % m_basesize;
    size_t ncap = m_cur->size - npos;
    size_t bpos = 0;

    while(len){
            if(ncap >= len){
                memcpy((char*)buf + bpos,m_cur->ptr + npos,len);
                if(m_cur->next && (npos + len) == m_cur->size){
                    m_cur = m_cur->next;
                }
                m_position += len;
                len = 0;
                break;
            }else {
                memcpy((char*)buf + bpos,m_cur->ptr + npos,ncap);
                m_cur = m_cur->next;
                len -= ncap;
                m_position += ncap;
                bpos += ncap;
                npos = 0;
                ncap = m_cur->size;
            }
    }
    if(m_position == m_size + 1){
        m_position = m_size;
    }

}
void Bytearray::clear(){
    Node* tmp = m_root->next;
    while(tmp){
        m_cur = tmp->next;
        delete tmp;
        tmp = m_cur;
    }
    m_position = 0;
    m_cur = m_root;
    m_size = 0;
    m_capacity = m_basesize;
}

void   Bytearray::addCapacity(size_t val){
        size_t cap = m_capacity - m_size;
        if(cap >= val){
            return;
        }
        val -= cap;
        size_t snode = val / m_basesize + ((val % m_basesize > 0) ? 1 : 0);

        Node* tmp = m_cur;
        while(tmp->next){
            tmp = tmp->next;
        }
        while(snode){
            Node* n = new Node(m_basesize);
            tmp->next = n;
            tmp = tmp->next;
            m_capacity += m_basesize;
            snode--;
        }
        if((m_size % m_basesize) == 0 && m_size != 0){
            m_cur = m_cur->next;
            //m_position += 1;
        }
}
size_t Bytearray::getCapacity(){
        return m_capacity;
}
size_t Bytearray::getReadsize(){        
        //return m_size - m_position ;
        return m_size - m_position + 1;
}
void   Bytearray::setPosition(size_t pos){
    if(pos > m_size || m_position <0){
        return;
    }
    if(pos == 0){
        m_position = pos;
        m_cur = m_root;
        return;
    }
    m_position = pos;
    size_t snode = m_position / m_basesize + ((m_position % m_basesize > 0) ? 1 : 0);
    m_cur=m_root;
    while(--snode){
        m_cur = m_cur->next;
    }
}

void Bytearray::readNoChange(void* buf,size_t position){
    size_t old_position = m_position;
    Node* old_cur = m_cur;

    setPosition(position);
    read(buf,m_size - position);
    m_position = old_position;
    m_cur = old_cur;
}
void Bytearray::writeTofile(std::string filename,size_t position){
        if(position > m_size){
            return;
        }
        size_t s = m_size - position;
        std::string str;
        str.resize(s);
        readNoChange(&str[0],position);
        
        std::ofstream fs;
        fs.open(filename,std::ios::trunc | std::ios::binary);
        if(!fs.is_open()){
            DZY_LOG_ERROR(g_logger) <<"open file="<<filename<<"  faild"<<"   errno="<<errno<<" errstr="<<strerror(errno);
            return;
        }
        fs.write(&str[0],str.size());
        fs.close();
}


void Bytearray::readFromfile(std::string filename){
    std::ifstream fs;
    fs.open(filename,std::ios::binary);
    char buf[4096];
    while(!fs.eof()){
        fs.read(buf,sizeof(buf));
        write(buf, fs.gcount());
    }
    fs.close();
}

}
