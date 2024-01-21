#ifndef DZY_UTIL_H__
#define DZY_UTIL_H__

#include <bits/stdint-uintn.h>
#include <sstream>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string>
#include <execinfo.h>
#include <vector>

namespace dzy{
#define DZY_ASSERT(x) \
    if(!(x)){  \
    DZY_LOG_ERROR(DZY_LOG_ROOT()) << "ASSERT: "<<#x << "\nbacktrace\n" \
        << dzy::BacktraceTostring(100); \
}
#define DZY_ASSERT2(x,w) \
    if(!(x)){  \
    DZY_LOG_ERROR(DZY_LOG_ROOT()) << "ASSERT: "<<#x <<"\n"<<"info:"<<#w "\nbacktrace\n" \
        << dzy::BacktraceTostring(100); \
}




pid_t Gettid();
uint32_t GetFiberId();

void Backtrace(std::vector<std::string>& bt,size_t size,size_t skip = 2);

std::string BacktraceTostring(size_t size,size_t skip = 2);


}

#endif
