#pragma once
#include "log.h"

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


