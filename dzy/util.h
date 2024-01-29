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

pid_t Gettid();
uint32_t GetFiberId();

void Backtrace(std::vector<std::string>& bt,size_t size,size_t skip = 2);

std::string BacktraceTostring(size_t size,size_t skip = 2);


}

#endif
