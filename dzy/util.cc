#include "util.h"
#include "log.h"
#include "fiber.h"
namespace dzy{

pid_t Gettid(){
    return syscall(SYS_gettid);
}
uint32_t GetFiberId(){
    return (uint32_t)Fiber::GetThis()->getid();
}

void Backtrace(std::vector<std::string>& bt,size_t size,size_t skip){
        void** array = (void**)malloc(sizeof(void*) * size);
        size_t s = ::backtrace(array,size);
        char** strings = ::backtrace_symbols(array, s);
        if(strings == nullptr){
            DZY_LOG_ERROR(DZY_LOG_ROOT()) << "backtrace_symbols() error";
            return;
        }
        for(size_t i = skip;i < s;i++){
            bt.push_back(strings[i]);
        }
}
std::string BacktraceTostring(size_t size,size_t skip){
        std::vector<std::string> bt;
        Backtrace(bt,size, skip);
        std::stringstream ss;
        for(size_t i = 0;i <bt.size();i++){
            ss << bt[i] << std::endl;
        }
        return ss.str();
}

uint64_t GetTimeMs(){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    uint64_t t = 0;
    t = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return t;
}
uint64_t GetTimeUs(){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    uint64_t t = 0;
    t = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
    return t;
}


}
