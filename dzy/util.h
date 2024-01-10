#pragma once
#include <bits/stdint-uintn.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

namespace dzy{
    pid_t Gettid(){
        return syscall(SYS_gettid);
    }
    uint32_t GetFiberId(){
        return (uint32_t)0;
    }

    template <class T>
    class Singleton {
        public:
            static T* GetInstance(){
                static T t;
                return  &t;
            }
    };



}
