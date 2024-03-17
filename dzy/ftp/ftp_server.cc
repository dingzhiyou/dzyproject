#include "ftp_server.h"
#include <cstring>
#include <ios>
#include <memory>
#include <string>
#include <fstream>

namespace dzy {
namespace ftp {
static Logger::ptr g_logger = DZY_LOG_ROOT();;

void FtpServer::handleClient(Socket::ptr sock) {
    size_t len = 1024 * 1024;
    std::shared_ptr<char> buff(new char[len],[](char* ptr){
        delete[] ptr;
    });

        int n = sock->recv(buff.get() ,len);
        std::string head(buff.get(),n);
        
       int pos =  head.find(' ');
       std::string method = head.substr(0,pos);

       std::string path = head.substr(pos+1, head.size() - pos - 2 - 1);
       DZY_LOG_INFO(g_logger) <<"method="<< method;
       DZY_LOG_INFO(g_logger) <<"path.size="<< path.size();


       if(strncasecmp(method.c_str(),"download",method.size()) == 0){
           std::ifstream fss(path);
        //   fss.open(path);
           if(fss) {
                    while(!fss.eof()){
                            fss.read(buff.get(), len);
                            DZY_LOG_INFO(g_logger) <<std::string(buff.get(),fss.gcount());
                            sock->send(buff.get(),fss.gcount());
                    }

           }else {
               DZY_LOG_INFO(g_logger) <<"fss is nullptr errno"<< errno 
                                      << "  errstr="<<strerror(errno);
           }
       }
       //TODO upload
       if(!strncasecmp(method.c_str(),"upload",method.size())){
                std::ofstream ofs;
                ofs.open(path,std::ios::trunc);
                if(!ofs) {
                        DZY_LOG_INFO(g_logger) << "error,errno="<<errno 
                                           << "   errstr="<<strerror(errno);
                        return;
                }
                DZY_LOG_INFO(g_logger) <<"==============";
                while(true) {
                        int rt = sock->recv(buff.get(),len);
                        if(rt == 0) {
                            ofs.close();
                            break;
                        }else if(rt == -1){
                            DZY_LOG_INFO(g_logger) << "error,errno="<<errno 
                                                   << "   errstr="<<strerror(errno);
                            break;
                        }
                        DZY_LOG_INFO(g_logger) << "read="<<std::string(buff.get(),rt);
                        ofs.write(buff.get(),rt);
                }
       }
}


}
}
