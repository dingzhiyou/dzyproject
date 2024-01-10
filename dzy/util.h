#ifndef DZY_UTIL_H__
#define DZY_UTIL_H__

#include <bits/stdint-uintn.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

namespace dzy{
pid_t Gettid();
uint32_t GetFiberId();



}

#endif
