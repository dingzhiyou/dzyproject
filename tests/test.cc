#include "../dzy/log.h"
#include "../dzy/util.h"

int main(int argc,char** argv){
  auto log = dzy::LoggerMgr::GetInstance()->GetRoot();
  DZY_LOG_FATAL(log)<<"111111111111111111";
   return  0;

}
