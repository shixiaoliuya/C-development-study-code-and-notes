#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>

int main() {
    // 1.创建监听的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0); // 使用TCP，因此用流式的
    if(fd == -1) {
        perror("socket");
        return -1;
    }

    // 2. 绑定本地的IP 和 port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999); // 绑定一个未被占用的端口
    // 如果我们给监听的套接字绑定一个0地址，就意味着它会自动地去读网卡对应的实际IP地址
    saddr.sin_addr.s_addr = INADDR_ANY; // 0 = 0.0.0.0。 0地址的意思是可以绑定本地的任意一个IP地址
    int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        return -1;
    }

    // 3. 设置监听
    ret = listen(fd, 128); // 可以设置一个比128小的整数，最大为128
    if(ret == -1) {
        perror("listen");
        return -1;
    }

    // 4. 阻塞并等待客户端的连接
    struct sockaddr_in caddr;
    int addrlen = sizeof(caddr);
    int cfd =accept(fd, (struct sockaddr*)&caddr, &addrlen);
    if(cfd == -1) {
        perror("accept");
        return -1;
    }

    // 连接建立成功，打印客户端的IP和端口信息
    char ip[32];
    printf("client IP: %s, port: %d\n", 
            inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip)),
            ntohs(caddr.sin_port));

    // 通信
    while(1) {
        // 接收数据
        char buff[1024];
        int len = recv(cfd, buff, sizeof(buff), 0);
        if(len > 0) {
            printf("clien(t say: %s\n", buff);
            send(cfd, buff, len, 0);
        }
        else if(len == 0) {
            printf("Client disconnects....\n");
            break;
        }
        else {
            perror("recv");
            break;
        }
    }

    // 关闭文件描述符
    close(fd);
    close(cfd);

    return 0;
}