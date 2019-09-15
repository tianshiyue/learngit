/*************************************************************************
	> File Name: client_init.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月08日 星期日 20时07分41秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define PORT 9999
#define IP "127.0.0.1"

int My_connect () {
    int cli_fd;
    cli_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_fd == -1){
        perror("socket\n");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    int  ret = (connect(cli_fd, (struct sockaddr *)&addr, sizeof(addr)));
    if (ret == -1){
        perror("connect\n");
        return -1;
    }
	return cli_fd;
}
