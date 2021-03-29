#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "include/reactor.h"

Reactor::Reactor() : loop(new EventLoop()) {}

Reactor::~Reactor() {}

void Reactor::init(int port)
{
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return;
    }

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    int ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    if (ret == -1) {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return ;
    }
    ret = listen(listenfd, 10);
    if (ret == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        return ;
    }
    setNonBlocking(listenfd);
    // 将监听的fd注册
    loop->register_handler(listenfd, READ_EVENT, [this](int listfd) {
        this->connectClient(listfd);
    });
}

void Reactor::connectClient(int fd)
{
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int sockfd = accept(fd, (struct sockaddr*)&client, &len);
    setNonBlocking(sockfd);
    if (sockfd<0) {
        printf("accept error\n");
    } else {
        // 添加连接fd,并且注册读事件
        loop->register_handler(sockfd, READ_EVENT, [this](int connfd) {
            this->recvFrom(connfd);
        });
    }
}

void Reactor::recvFrom(int fd)
{
    char recv_buf[4096];
    int n_left = 4096;
    int n_read = 0;
    while (n_left > 0) {
        n_read = read(fd, recv_buf+n_read, n_left);
        if (n_read < 0) {
            if (errno == EINTR) {
                n_read=0;
            } else if (errno==EAGAIN) {//读取完成
                printf("recv message : %s\n", recv_buf);

                char send_buf[20] = "recv successed!";
                int send_len = sizeof(send_buf);

                while(send_len>0){
                    int len=write(fd,send_buf,send_len);
                    if(len<0){
                        if(errno == EAGAIN) {  // 写完成
                            close(fd);
                            loop->remove_handler(fd);
                            break;
                        }
                    }
                    else if(len == 0) {
                        close(fd);  // 先关闭套接字
                        loop->remove_handler(fd);
                        return ;
                    } else {
                        send_len-=len;
                    }
                }
                break;
            }
        } else if(n_read == 0) {
            close(fd);
            loop->remove_handler(fd);
            break;
        }
        n_left -= n_read;
    }
}

void Reactor::run()
{
    loop->run();
}

int Reactor::setNonBlocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void Reactor::setReuseAddr(int sockfd)
{
    assert(sockfd >= 0);
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
}