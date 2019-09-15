/*************************************************************************
	> File Name: client_main.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月08日 星期日 23时24分44秒
 ************************************************************************/

#include <stdio.h>
#include "client_init.h"
#include "client_menu.h"
int main()
{
    int client_fd = My_connect();
    if (client_fd != -1) {
        login_menuoi(client_fd);
    }
}
