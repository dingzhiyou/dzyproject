#include "util.h"

namespace dzy{

    pid_t Gettid(){
        return syscall(SYS_gettid);
    }
    uint32_t GetFiberId(){
        return (uint32_t)0;
    }




}
