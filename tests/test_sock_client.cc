#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 80
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_size = sizeof(client_address);
    char buffer[BUFFER_SIZE] = "xxxxxxxxxxxxxxxaaaaaaaaa";

    // 创建服务器套接字
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    // 绑定套接字到指定地址和端口
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 接受连接并发送数据
    while (1) {
        // 接受连接
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket == -1) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // 向客户端发送数据
        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Send failed");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        printf("Data sent to client: %s\n", buffer);

        // 关闭客户端套接字
        close(client_socket);
    }

    // 关闭服务器套接字
    close(server_socket);

    return 0;
}

