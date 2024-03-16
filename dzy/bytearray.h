#include <memory>
#include <string>
#include <stdint.h>

namespace dzy {

class Bytearray {
public:
    typedef std::shared_ptr<Bytearray> ptr;

    class Node {
        public:
            Node(size_t size);
            Node();

            Node* next;
            char* ptr;
            size_t size;
            uint64_t id;
    };
    Bytearray(size_t basesize);
    ~Bytearray();
    //write
    void writeFint8(int8_t val);
    void writeFuint8(uint8_t val);
    void writeFint16(int16_t val);
    void writeFuint16(uint16_t val);
    void writeFint32(int32_t val);
    void writeFuint32(uint32_t val);
    void writeFint64(int64_t val);
    void writeFuint64(uint64_t val);

    void writeFloat(float val);
    void writeDouble(double val);

    void writeStringF16(std::string& val);
    void writeStringF32(std::string& val);
    void writeStringF64(std::string& val);
    void writeStringVar(std::string& val);

    void writeUint32(uint32_t val);
    void writeUint64(uint64_t val);

    //read
    int8_t   readFint8();
    uint8_t  readFuint8();
    int16_t  readFint16();
    uint16_t readFuint16();
    int32_t  readFint32();
    uint32_t readFuint32();
    int64_t  readFint64();
    uint64_t readFuint64();

    float    readFloat();
    double   readDouble();

    std::string readStringF16();
    std::string readStringF32();
    std::string readStringF64();
    std::string readStringVar();

    uint32_t  readUint32();
    uint64_t  readUint64();

    void write(void* buf,size_t len);
    void read(void* buf,size_t len);
    void readNoChange(void* buf,size_t position);
    void clear();

    void writeTofile(std::string filename,size_t position);
    void readFromfile(std::string filename);
    size_t getSize()const {return m_size;}
    
    void setPosition(size_t pos);
    void addCapacity(size_t val);
    size_t getCapacity();
    size_t getReadsize();
public:
    uint64_t m_position;
    uint64_t m_capacity;
    uint64_t m_size;
    uint64_t m_basesize;
    Node* m_root;
    Node* m_cur;

};

}
