#include "../dzy/log.h"
#include "../dzy/config.h"


int main(){
    auto logger = dzy::LoggerMgr::GetInstance()->GetLogger("system");
    DZY_LOG_FATAL(logger) << "hello system";

}
