#include "../dzy/bytearray.h"
#include "../dzy/log.h"
#include <bits/stdint-uintn.h>
#include <cstdlib>
#include <vector>
#include "../dzy/macro.h"

static dzy::Logger::ptr g_logger = DZY_LOG_ROOT();

int main(){
    dzy::Bytearray::ptr Ba(new dzy::Bytearray(9));
    std::string str("dyagfuvwfvwyigfywvcuwiw dwyugfuvw fwygfv dwohdawf");
    Ba->writeStringF16(str);
    Ba->setPosition(0);
    std::string str1 = Ba->readStringF16();
    DZY_LOG_DEBUG(g_logger) << "str1:"<<str1;
    //std::vector<int64_t> v;
    //for(int i =0 ;i < 100 ;i++){
    //    int64_t value = i;
    //    v.push_back(value);
    //    Ba->writeFint64(value);
    //}

   //std::string name = "./file";
   //Ba->writeTofile(name,0);

    //Ba->setPosition(0);
   //// DZY_LOG_ERROR(g_logger) <<*(int64_t*)(Ba->m_root->ptr);
    //for(int i =0 ;i < 100;i++){
    //    int64_t value = Ba->readFint64();
    //    DZY_LOG_ERROR(g_logger) <<"m_position="<<Ba->m_position;
    //    DZY_LOG_DEBUG(g_logger) <<"i="<<i<<"   " <<value<<"--"<<v[i];
    //}
    //
    //DZY_LOG_ERROR(g_logger) <<"m_size="<<Ba->m_size;
    //dzy::Bytearray::ptr Ba2(new dzy::Bytearray(3));
    //Ba2->readFromfile("./file");
    //Ba2->setPosition(0);
    //for(int i =0 ;i < 100;i++){
    //    int64_t value = Ba2->readFint64();
    //    DZY_LOG_DEBUG(g_logger) <<value;
    //}
   //Ba->writeFint64(10);
   //Ba->writeFint64(20);
   //Ba->writeFint64(30);
   //Ba->setPosition(0);

   // DZY_LOG_DEBUG(g_logger)<<"m_position"<<Ba->m_position;
   //int64_t value = Ba->readFint64();
   // DZY_LOG_DEBUG(g_logger)<<value<<"  m_position"<<Ba->m_position;
   //int64_t value2 = Ba->readFint64();
   // DZY_LOG_DEBUG(g_logger)<<value2<<" m_position"<<Ba->m_position;
   //int64_t value3 = Ba->readFint64();
   // DZY_LOG_DEBUG(g_logger)<<value3<<"  m_position"<<Ba->m_position;
   // return 0;
}
