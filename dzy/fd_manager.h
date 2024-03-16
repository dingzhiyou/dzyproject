#pragma once

#include "singleton.h"
#include <bits/stdint-uintn.h>
#include <memory>
#include <functional>
#include <type_traits>
#include <vector>

namespace dzy {

class FdCtx : public std::enable_shared_from_this<FdCtx> {
public:
        typedef std::shared_ptr<FdCtx> ptr;
        FdCtx(int fd);
        void init();
        bool isSocket();
        uint64_t getTimeOut(int type);
        void setTimeOut(int type,uint64_t timeout);
        bool isClose();
        bool getUserNonblock()const {return m_userNonblock;}
        bool getSysNonblock()const {return m_sysNonblock;}
private:
    int m_fd = -1;
    bool m_isinit = false;
    bool m_issocket = false;

    uint64_t  m_sendTimeOut = -1;
    uint64_t  m_recvTimeOut = -1;
    bool m_userNonblock = false;
    bool m_sysNonblock = false;
    

};
class FdManager {
public:
        FdCtx::ptr get(int fd,bool auto_create = true);
        bool del(int fd);
private:
    std::vector<FdCtx::ptr>  m_fdCtx;
};
typedef Singleton<FdManager> FdMgr;

}
