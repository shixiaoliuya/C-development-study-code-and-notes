#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>

int main() {
    // 1.创建通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0); // 使用TCP，因此用流式的
    if(fd == -1) {
        perror("socket");
        return -1;
    }

    // 2. 绑定服务器的IP 和 port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999); // 绑定一个未被占用的端口
    inet_pton(AF_INET, "192.168.241.128", &saddr.sin_addr.s_addr);
    int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret == -1) {
        perror("connect");
        return -1;
    }

    int number = 0;
    // 通信
    while(1) {
        // 发送数据
        char buff[1024]; // 要发送的数据存入buff
        sprintf(buff, "hello server, %d...\n", number++);
        send(fd, buff, strlen(buff) + 1, 0);

        // 接收数据
        memset(buff, 0, sizeof(buff));
        int len = recv(fd, buff, sizeof(buff), 0);
        if(len > 0) {
            printf("clien(t say: %s\n", buff);
        }
        else if(len == 0) {
            printf("Server disconnects....\n");
            break;
        }
        else {
            perror("recv");
            break;
        }
        sleep(1);
    }

    // 关闭文件描述符
    close(fd);

    return 0;
}