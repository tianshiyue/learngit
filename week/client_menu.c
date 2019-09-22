/*************************************************************************
	> File Name: client_menu.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月08日 星期日 21时10分21秒
 ************************************************************************/
#include "define.h"

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <fcntl.h>

void *recv_PACK(void *);

void clear_input_buffer()
{
    while (getchar() != '\n')
        ;
}

// int client_fd;
char name[USERNAME_LEN];

// 注册
int  client_regist (int client_fd)
{
    PACK pack;
    pack.type = REGIST;
    printf("\t\t请输入用户名:");
    scanf("%s", pack.username);
    printf("\t\t请输入密码:");
    scanf("%s", pack.password);
    int ret = send(client_fd, &pack, sizeof(PACK), 0 );
    if (ret < 0) {
        perror("client_regist:send\n");
    }
    while(1) {
        memset(&pack, 0, sizeof(PACK));
        ret = recv(client_fd, &pack, sizeof(PACK), 0);
        if (ret < 0) {
            perror("regist_recv\n");
        }
        else {
            if (pack.type == 0) {
                printf("\t\t%s注册成功\n", pack.username);
            }
            if (pack.type == -1) {
                printf("\t\t用户名已存在 注册失败:\n");
            }
        }
        return INITA;
    }
}

void *recv_pack(void *);
void use_menuoi(int);


// 登录
int  client_login(int client_fd)
{
    PACK pack;
    pack.type = LOGIN;
    pthread_t recv_msg;
    printf("\t\t请输入用户名:");
    scanf("%s", pack.username);
    strcpy(name, pack.username);
    printf("\t\t请输入密码:");
    scanf("%s", pack.password);
    int ret = send(client_fd, &pack, sizeof(PACK), 0 );
    if (ret < 0){
        perror("client_login:send\n");
    }
    while(1) {
        memset(&pack, 0, sizeof(PACK));
        ret = recv(client_fd, &pack, sizeof(PACK), 0);
        if (ret < 0){
            perror("login_recv\n");
        }
        else {
            if (pack.type == 0) {
                printf("\t\t%s登录成功\n", pack.username);
                pthread_create(&recv_msg, NULL, recv_PACK, &client_fd);
                use_menuoi(client_fd);
                return INITA;
            }
            if (pack.type == -1) {
                printf("\t\t用户名不存在或密码错误 登录失败:\n");
                return INITA;
            }
        }
    }
}

int client_exit(int client_fd)
{
    PACK pack;
    strcpy(pack.username, name);
    pack.type = EXITB;
    if(send(client_fd, &pack, sizeof(PACK),0)<0) {
        perror("client_exit\n");
    }
    return INITA;
}

// 加好友
int client_addfd(int client_fd)
{
    PACK pack;
    memset(&pack, 0, sizeof(PACK));
    // 将自己的用户名写进包里
    strcpy(pack.username, name);
    pack.type = ADD_FD;
    printf("\t\t请输入您需要添加的用户名:");
    scanf("%s", pack.send_username);;
    if(strcmp(pack.username, pack.send_username) == 0) {
        printf("\t\t对不起 自己不能添加自己为好友!\n");
    } else {
        if(send(client_fd, &pack, sizeof(PACK), 0) < 0) {
            perror("client_addfd:send\n");
        }
    }
    return  INITB;
}

int client_delfd(int client_fd)
{
    PACK pack;
    pack.type = DEL_FD;
    strcpy(pack.username, name);
    printf("\t\t请输入您需要删除好友的用户名:");
    scanf("%s", pack.send_username);
    if(strcmp(name, pack.send_username) == 0) {
        printf("\t\t对不起，自己不能删除自己\n");
    }
    if(send(client_fd, &pack, sizeof(PACK), 0) < 0) {
        perror("client_delfd:Send\n");
    }
    return INITB;
}

int client_fd_list(int client_fd) 
{
    PACK pack;
    strcpy(pack.username, name);
    pack.type = FD_LIST;
    if(send(client_fd, &pack, sizeof(PACK), 0) < 0) {
        perror("client_fd_list:error\n");
    }
    return INITB;
}

int client_online_fd_list(int client_fd)
{
    PACK pack;
    strcpy(pack.username, name);
    if(send(client_fd, &pack, sizeof(PACK), 0) < 0) {
        perror("client_online_fd_list:send\n");
    }
    return INITB;
}


int client_chat_fd(int client_fd)
{
    int t = 1;
    PACK pack;
    memset(&pack, 0, sizeof(PACK));
    pack.type = CHAT_FD;
    strcpy(pack.username, name);
    printf("\t\t请输入您需要聊天的好友用户名:");
    scanf("%s", pack.send_username);
    if(strcmp(name, pack.send_username) == 0) {
        printf("\t\t对不起，自己不能和自己聊天\n");
        return INITB;
    }
    while(t != 0) {
        printf("\t\t请输入您需要聊天的内容:");
        scanf("%s", pack.mess);
        t = strcmp("bye", pack.mess);
        if(t != 0) {
            int ret = send(client_fd, &pack, sizeof(PACK), 0);
            if(ret < 0) {
                perror("client_chat_fd:send\n");
            }
        }
    }
    return INITB;
}

int client_creat_gp(int client_fd)
{
    PACK pack;
    printf("\t\t请输入您创建的群名:");
    scanf("%s", pack.send_username);
    strcpy(pack.username, name);
    pack.type = CREAT_GP;
    int ret = send(client_fd, &pack, sizeof(PACK), 0);
    if(ret < 0) {
        perror("client_creat_gp:send\n");
    }
    return INITB;
}

int client_fd_chatstore(int client_fd)
{
    PACK pack;
    pack.type = FD_CHATSTORE;
    strcpy(pack.username, name);
    printf("\t\t请输入您要查看的好友用户名:");
    scanf("%s", pack.send_username);
    int ret = send(client_fd, &pack, sizeof(PACK), 0);
    if(ret < 0) {
        perror("client_fd_chatstore:send\n");
    }
    return INITB;
}

int client_join_gp(int client_fd)
{
    PACK pack;
    strcpy(pack.username, name);
    printf("\t\t请输入您想加入的群名:");
    scanf("%s", pack.send_username);
    pack.type = JOIN_GP;
    int ret = send(client_fd, &pack, sizeof(PACK), 0);
    if(ret < 0) {
        perror("client_join_gp:send\n");
    }
    return INITB;
}

int client_quit_gp(int client_fd)
{
    PACK pack;
    strcpy(pack.username, name);
    printf("\t\t请输入您想要退出的群名:");
    scanf("%s", pack.send_username);
    pack.type = QUIT_GP;
    int ret = send(client_fd, &pack, sizeof(PACK), 0);
    if(ret < 0) {
        perror("client_quit_gp:send\n");
    }
    return INITB;
}

int client_chat_gp(int client_fd)
{
    PACK pack;
    strcpy(pack.username, name);
    pack.type = CHAT_GP;
    int t = 1;
    while(t != 0) {
        printf("\t\t请输入您想要聊天的内容:");
        scanf("%s", pack.mess);
        t = strcmp("bye", pack.mess);
        if(t != 0) {
            printf("7&&&&&&&&&7\n");
            int ret = send(client_fd, &pack, sizeof(PACK), 0);
            if(ret < 0) {
                perror("client_chat_gp:send\n");
            }
        }
    }
    return INITB;
}

int client_gp_list(int client_fd)
{
    PACK pack;
    pack.type = GP_LIST;
    strcpy(pack.username, name);
    int ret = send(client_fd, &pack, sizeof(PACK), 0);
    if(ret < 0) {
        perror("client_gp_list:send\n");
    }
    return INITB;
}

int client_gp_user_list(int client_fd)
{
    PACK pack;
    pack.type = GP_USER_LIST;
    strcpy(pack.username, name);
    printf("\t\t请输入您想要查看的群名:");
    scanf("%s", pack.send_username);
    int ret = send(client_fd, &pack, sizeof(PACK), 0);
    if(ret < 0) {
        perror("client_gp_user_list:send\n");
    }
    return INITB;
}

int client_send_file(int client_fd)
{
    PACK pack;
    pack.type = SEND_FILE;
    strcpy(pack.username, name);
    printf("\t\t请输入您要发送的好友用户名:");
    scanf("%s", pack.send_username);
    char send_username[USERNAME_LEN];
    strcpy(send_username, pack.send_username);
    printf("\t\t请输入您所要发送文件的绝对路径:");
    char path[100];
    // buf结构体
    struct stat buf;
    int fd;
    int ret;
    scanf("%s", path);
    // lstat获取文件的一些信息
    if(lstat(path, &buf) == -1) {
        printf("\t\t输入的文件路径错误\n");
        return INITB;
    }
    // st_mode 文件的类型和权限
    if(S_ISDIR(buf.st_mode)) {
        printf("\t\t您输入的是一个目录\n");
        return INITB;
    }
    if((fd = open(path, O_RDONLY)) == -1) {
        perror("client_send_file:open\n");
        return INITB;
    }
    // read返回每次读取到的字节数
    while((ret = read(fd, pack.mess, sizeof(pack.mess)) < 0)) {
        if((send(client_fd, &pack, sizeof(PACK), 0)) < 0) {
            perror("client_send_file:send\n");
        }
        memset(&pack, 0, sizeof(PACK));
        pack.type = SEND_FILE;
        strcpy(pack.username, name);
        strcpy(pack.send_username, send_username);
    }
    close(fd);
    return INITB;
}

int client_gp_chatstore(int client_fd)
{
    PACK pack;
    strcpy(pack.username, name);
    pack.type = GP_CHATSTORE;
    printf("\t\t请输入您想要查看的群名:");
    scanf("%s", pack.send_username);
    int ret = send(client_fd, &pack, sizeof(PACK), 0);
    if(ret < 0) {
        perror("client_gp_chatstore:send\n");
    }
    return INITB;
}

int use_menu(int client_fd)
{
    time_t Time = time(NULL);
	char *buf = ctime(&Time);
	printf("\t\t%s", buf);
	printf("\t\t|****************************|\n");
	printf("\t\t|           主菜单           |\n");
	printf("\t\t|****************************|\n");
	printf("\t\t|      1、添加好友           |\n");
	printf("\t\t|      2、删除好友           |\n");
	printf("\t\t|      3、好友聊天           |\n");
	printf("\t\t|      4、好友列表           |\n");
	printf("\t\t|      5、聊天记录           |\n");
	printf("\t\t|      6、创建群聊           |\n");
	printf("\t\t|      7、加入群聊           |\n");
	printf("\t\t|      8、退出群聊           |\n");
	printf("\t\t|      9、群聊天             |\n");
	printf("\t\t|      10、群列表            |\n");
	printf("\t\t|      11、查看群成员        |\n");
	printf("\t\t|      12、群聊天记录        |\n");
	printf("\t\t|      13、发送文件          |\n");
	printf("\t\t|      14、好友申请          |\n");
	printf("\t\t|      15、在线好友列表      |\n");
	printf("\t\t|      0、退出               |\n");
	printf("\t\t|****************************|\n");
	printf("\t\t请输入您要进行的操作:");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            return ADD_FD;
        case 2:
            return DEL_FD;
        case 3:
            return CHAT_FD;
        case 4:
            return FD_LIST;
        case 5:
            return FD_CHATSTORE;
        case 6:
            return CREAT_GP;
        case 7:
            return JOIN_GP;
        case 8:
            return QUIT_GP;
        case 9:
            return CHAT_GP;
        case 10:
            return GP_LIST;
        case 11:
            return GP_USER_LIST;
        case 12:
            return GP_CHATSTORE;
        case 13:
            return SEND_FILE;
        case 14:
            return MESS_RECV_ADDFD;
        case 15:
            return ONLINE_FD_LIST;
        case 0:
            return EXITB;
        default :
            printf("\t\t输入错误\n");
            break;
    }
    return INITB;
}

int client_mess_recv_addfd(int);

void use_menuoi(int client_fd)
{
    int  status = INITB;
    while (status != EXITB) {
        switch (status) {
            case INITB:
                status = use_menu(client_fd);
                break;
            case ADD_FD:
                status = client_addfd(client_fd);
                break;
            case DEL_FD:
                status = client_delfd(client_fd);
                break;
            case FD_LIST:
                status = client_fd_list(client_fd);
                break;
            case FD_CHATSTORE:
                status = client_fd_chatstore(client_fd);
                break;
            case MESS_RECV_ADDFD:
                status = client_mess_recv_addfd(client_fd);
                break;
            case CHAT_FD:
                status = client_chat_fd(client_fd);
                break;
            case CREAT_GP:
                status = client_creat_gp(client_fd);
                break;
            case JOIN_GP:
                status = client_join_gp(client_fd);
                break;
            case QUIT_GP:
                status = client_quit_gp(client_fd);
                break;
            case CHAT_GP:
                status = client_chat_gp(client_fd);
                break;
            case GP_LIST:
                status = client_gp_list(client_fd);
                break;
            case GP_USER_LIST:
                status = client_gp_user_list(client_fd);
                break;
            case GP_CHATSTORE:
                status = client_gp_chatstore(client_fd);
                break;
            case SEND_FILE:
                status = client_send_file(client_fd);
                break;
            case ONLINE_FD_LIST:
                status = client_online_fd_list(client_fd);
                break;
            case EXITB:
                break;
            default:
                printf("status unknown: %d", status);   
        }
        clear_input_buffer();
    }
    if(status == EXITB) {
        status = client_exit(client_fd);
    }
}

int  login_menu(int client_fd)
{
    time_t Time = time(NULL);
	char *buf = ctime(&Time);
	printf("\t\t%s", buf);
	printf("\t\t|****************************|\n");
	printf("\t\t|          欢迎使用          |\n");
	printf("\t\t|****************************|\n");
	printf("\t\t|       1、用户注册          |\n");
	printf("\t\t|       2、用户登录          |\n");
	printf("\t\t|       0、退出              |\n");
	printf("\t\t|****************************|\n");
	printf("\t\t请输入您要进行的操作:");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            return REGIST;
        case 2:
            return LOGIN;
        case 0:
            return EXITA;
        default:
            printf("\t\t输入错误\n");
            break;
    }
    return INITA;
}

void login_menuoi(int client_fd)
{
    int  status = INITA;
    pthread_t recv_msg;
    while (status != EXITA) {
        switch (status) {
            case INITA:
                status = login_menu(client_fd);
                break;
            case REGIST:
                status = client_regist(client_fd);
                break;;
            case LOGIN:
                status = client_login(client_fd);
                break;
            case EXITA:
                break;
            default:
                printf("status unknown: %d", status);
        }
        clear_input_buffer();
    }
}


void client_recv_addfd(PACK);
void agree_addfd(PACK);
void print_fd_list(PACK);
void print_chat_fd(PACK);
void print_fd_chatstore(PACK);
void print_creat_gp(PACK);
void print_join_gp(PACK);
void print_quit_gp(PACK);
void print_chat_gp(PACK);
void print_gp_list(PACK);
void print_gp_user_list(PACK);
void print_gp_chatstore(PACK);
void recv_file(PACK);
void recv_deal_addfd(PACK);
void recv_deal_chat_fd(PACK);

void *recv_PACK(void *client_fd)
{
    int fd = *(int *)client_fd;
    PACK pack;
    while (1) {
        memset(&pack, 0, sizeof(PACK));
        int ret;
        if((ret = recv(fd, &pack, sizeof(PACK), MSG_WAITALL)) < 0) {
            perror("client_recv_pack:\n");
        }
        switch(pack.type) {
            case RECV_ADDFD:
                client_recv_addfd(pack);
                break;
            case AGREE_ADDFD:
                agree_addfd(pack);
                break;
            case RECV_FD_LIST:
                print_fd_list(pack);
                break;
            case RECV_CHAT_FD:
                print_chat_fd(pack);
                break;
            case RECV_FD_CHATSTORE:
                print_fd_chatstore(pack);
                break;
            case RECV_CREAT_GP:
                print_creat_gp(pack);
                break;
            case RECV_JOIN_GP:
                print_join_gp(pack);
                break;
            case RECV_QUIT_GP:
                print_quit_gp(pack);
                break;
            case RECV_CHAT_GP:
                print_chat_gp(pack);
                break;
            case RECV_GP_LIST:
                print_gp_list(pack);
                break;
            case RECV_GP_USER_LIST:
                print_gp_user_list(pack);
                break;
            case RECV_GP_CHATSTORE:
                print_gp_chatstore(pack);
                break;
            case RECV_FILE:
                recv_file(pack);
                break;
            case DEAL_ADDFD:
                recv_deal_addfd(pack);
                break;
            case DEAL_CHAT_FD:
                recv_deal_chat_fd(pack);
                break;
        }
    }
}

typedef struct recv_addfd {
    char name[USERNAME_LEN];
    struct recv_addfd *next;
} recv_addfd;

recv_addfd *phead = NULL, *pend;

void client_recv_addfd(PACK pack) 
{
    if(phead == NULL) {
        phead = (struct recv_addfd *)malloc(sizeof(struct recv_addfd));
        phead->next = NULL;
        pend = phead;
    }
    recv_addfd *ptemp;
    ptemp = (struct recv_addfd *)malloc(sizeof(struct recv_addfd));
    strcpy(ptemp->name, pack.username);
    ptemp->next = NULL;
    pend->next = ptemp;
    pend = ptemp;
    printf("\n");
    printf("\t\t您有一条好友申请 请及时前往好友申请消息盒子查看\n");
}

int  client_mess_recv_addfd(int client_fd)
{
    PACK pack;
    if(phead == NULL) {
        printf("\t\t您没有好友申请\n");
    } else {
        recv_addfd *p1, *p2;
        p1 = phead;
        while(p1->next != NULL) {
            p2 = p1->next;
            printf("\t\t%s 请求加您为好友:同意请按1 拒绝请按0\n", p2->name);
            int t;
            scanf("%d", &t);
            memset(&pack, 0, sizeof(PACK));
            pack.type = MESS_RECV_ADDFD;
            strcpy(pack.username, name);
            strcpy(pack.send_username, p2->name);
            if(t == 1) {
                strcpy(pack.mess, "success");
                if((send(client_fd, &pack, sizeof(PACK), 0)) < 0) {
                    perror("client_mess_recv_addfd:send\n");
                }
            } else {
                strcpy(pack.mess, "fail");
                if((send(client_fd, &pack, sizeof(PACK), 0)) < 0) {
                    perror("client_mess_recv_addfd:send\n");
                }
            }
            p1->next = p2->next;
            free(p2);
        }
        free(p1);
        phead = NULL;
    }
    return INITB;
}

void agree_addfd(PACK pack) 
{
    if(strcmp(pack.mess, "success") == 0) {
        printf("\t\t%s已同意您的好友请求\n", pack.username);
    } else {
        printf("\t\t%s已拒绝您的好友请求\n", pack.username);
    }
}

void recv_deal_addfd(PACK pack) 
{
    printf("\n");
    if(strcmp("fail", pack.mess) == 0) {
        printf("\t\t您和该用户以是好友或该用户不存在\n");
    }
}

void print_fd_list(PACK pack)
{
    int t = strcmp("bye", pack.send_username);
    //printf("您的好友列表为:\n");
    if(t != 0) {
        printf("\t%s\n", pack.send_username);
    }
}


void recv_deal_chat_fd(PACK pack)
{
    printf("\n");
    if(strcmp("fail", pack.mess) == 0) {
        printf("\t\t您与该用户并不是好友或者该用户不存在，发送失败\n");
    }
    /*printf("请输入bye退出:");
    char buff[5];
    scanf("%s", buff);*/
}

void print_chat_fd(PACK pack) 
{
    printf("\n");
    printf("%s给您发来了一条消息:%s\n", pack.username, pack.mess);
}

void print_fd_chatstore(PACK pack)
{
    printf("\n");
    printf("发送人:%s  接收人:%s  信息:%s\n", pack.username, pack.send_username, pack.mess);
}

void print_creat_gp(PACK pack)
{
   // printf("pack.send_username = %s\n", pack.send_username);
    //printf("pack.send_mess = %s\n", pack.mess);
    if(strcmp("success", pack.mess) == 0) {
        printf("群%s创建成功\n", pack.send_username);
    } else {
        printf("群%s创建失败\n", pack.send_username);
    }
}

void print_join_gp(PACK pack)
{
    printf("\n");
    if(strcmp("success", pack.mess) == 0) {
        printf("\t\t群%s加入成功\n", pack.send_username);
    } else {
        printf("\t\t群%s不存在 加入失败", pack.send_username);
    }
}

void print_quit_gp(PACK pack)
{
    printf("\n");
    if(strcmp("success", pack.mess) == 0) {
        printf("\t\t群%s退出成功\n", pack.send_username);
    } else {
        printf("\t\t群%s不存或者您并不在该群 退出失败\n", pack.send_username);
    }
}

void print_chat_gp(PACK pack)
{
    printf("\n");
    printf("群%s中的用户%s发来了一条群消息:%s\n", pack.send_username, pack.username, pack.mess);
}

void print_gp_list(PACK pack)
{
    printf("\n");
   // printf("您的群列表为:\n");
    int t = strcmp(pack.send_username, "bye");
    if(t != 0) {
        printf("\t%s\n", pack.send_username);
    }
}

void print_gp_user_list(PACK pack)
{
    printf("\n");
    //printf("群%s的成员有:\n", pack.username);
    int t = strcmp(pack.send_username, "bye");
    if(t != 0) {
        printf("\t   %s\n", pack.send_username);
    }
}

void print_gp_chatstore(PACK pack)
{
    printf("\n");
    printf("发件人:%s   信息:%s\n", pack.username, pack.mess);
}

void recv_file(PACK pack)
{
    PACK send_pack;
    int fd;
    if((fd = open("recv_test", O_RDWR|O_CREAT|O_APPEND, S_IRUSR| S_IWUSR)) == -1) {
        perror("recv_file:open\n");
    }
    int ret;
    if( (ret = write(fd, pack.mess, sizeof(pack.mess))) < 0) {
        perror("recv_file:write\n");
    }
    close(fd);
}
