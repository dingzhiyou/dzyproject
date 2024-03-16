#include "../dzy/addr.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

static uint64_t g_count = 0;
int main() {
    
    const char* server_ip = "127.0.0.1";
    int port = 8020;
    while(true) {
                // 创建套接字对象
                int client_socket = socket(AF_INET, SOCK_STREAM, 0);
                if (client_socket == -1) {
                    std::cout << "无法创建套接字" << std::endl;
                    return 1;
                }

                // 构建服务器地址结构
                struct sockaddr_in server_addr;
                server_addr.sin_family = AF_INET;
                server_addr.sin_port = htons(port);
                inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

                // 连接到服务器
                if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
                    std::cerr << "连接到本地端口8020失败" << std::endl;
                    close(client_socket);
                    return 1;
                }
                std::cout << g_count++ << std::endl;
           
    }

    return 0;
}
