#include "../dzy/ftp/ftp_server.h"
#include "../dzy/iomanager.h"

void run () {
        dzy::ftp::FtpServer::ptr ftp_server(new dzy::ftp::FtpServer);
        dzy::IPv4Address::ptr addr = dzy::IPv4Address::Create("127.0.0.1",8030);
        ftp_server->bind(addr);
        ftp_server->start();
}

int main () {
    dzy::IOManager iom(2);
    iom.schedule(run);

    return 0;
}
