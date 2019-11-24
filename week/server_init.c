/*************************************************************************
	> File Name: server_init.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月08日 星期日 20时11分20秒
 ************************************************************************/
#include <pthread.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "MYSQL.h"
#include "server_deal.h"
 
#define SERV_PORT       9999
#define IP              "0.0.0.0" 
#define EPOLL_MAX       1000
#define LISTENMAX       1000


int server_init()
{
    int sock_fd, epoll_fd, fd_num, conn_fd;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    struct epoll_event ev, events[LISTENMAX];
    int cli_len = sizeof(struct sockaddr_in);

    Init_mysql();
    
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("创建socket失败");
        exit(-1);
    }

    int optval = 1;
    if ((setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)))<0) {
        perror("套接字重新绑定失败");

    }

    memset(&serv_addr, 0,sizeof(struct sockaddr_in));
    serv_addr.sin_family =  AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    
    if ((bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)))<0) {
        perror("绑定失败");
        exit(-1);
    }
    // epoll_create创建一个epoll文件描述符
    epoll_fd = epoll_create(EPOLL_MAX);
    ev.data.fd = sock_fd;
    // ev.events设置要处理的事件类型
    ev.events = EPOLLIN;
    //epoll_ctl用来添加修改删除需要监听的文件描述符
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &ev);


    if ((listen(sock_fd, 10)) < 0) {
        perror("设置监听失败");
        exit(-1);
    }
    printf("listen success\n");
    while(1) {
        //epoll_wait用来接收发生在被监听的描述符
        fd_num = epoll_wait(epoll_fd, events, EPOLL_MAX, 1000);
        for (int i = 0; i < fd_num; i++)
        {
            // 有新的连接
            if (events[i].data.fd == sock_fd)
            {
                conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
                if (conn_fd < 0)
                {
                    perror("连接失败");
                }
                ev.data.fd = conn_fd;
                ev.events = EPOLLIN;
                // 将新的fd添加到epoll的监听队列中去
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev);

            }
            else if (events[i].events & EPOLLIN) {
               recv_pack(events[i].data.fd);
            }
        }
    }
}


