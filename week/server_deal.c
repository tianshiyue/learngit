/*************************************************************************
	> File Name: server_deal.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月08日 星期日 20时24分15秒
 ************************************************************************/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "MYSQL.h"
#include "define.h"

void deal_pack(PACK, int);
void deal_regist(PACK);
void deal_login(PACK, int);
void deal_exit(PACK);
void deal_addfd(PACK);
void deal_recv_addfd(PACK);
void deal_del_fd(PACK);
void deal_fd_list(PACK);
void deal_chat_fd(PACK);
void deal_fd_chatstore(PACK);
void deal_creat_gp(PACK);
void deal_join_gp(PACK);
void deal_quit_gp(PACK);
void deal_chat_gp(PACK);
void deal_gp_list(PACK);
void deal_gp_user_list(PACK);
void deal_gp_chat_store(PACK);
void deal_send_file(PACK);
void deal_online_fd_list(PACK);
void deal_untalk_gp(PACK);
void deal1_untalk_gp(PACK);
void deal_invite_user(PACK);
void deal_exit_gp(PACK);
void deal_black_fd(PACK);
void deal_exit_black_fd(PACK);

static int cli_fd;

void recv_pack(int conn_fd)
{
    PACK pack;
    cli_fd = conn_fd;
    int ret;
    if((ret = recv(cli_fd, &pack, sizeof(PACK), 0)) < 0){
        perror("recv");
    }
    if (ret == 0) {
        return ;
    }
    deal_pack(pack, cli_fd);
} 

void deal_pack(PACK pack, int cli_fd) 
{
    int type = pack.type;
    switch(type) {
        case REGIST:
            deal_regist(pack);
            break;
        case LOGIN:
            deal_login(pack, cli_fd);
            break;
        case EXITB:
            deal_exit(pack);
            break;
        case ADD_FD:
            deal_addfd(pack);
            break;
        case MESS_RECV_ADDFD:
            deal_recv_addfd(pack);
            break;
        case DEL_FD:
            deal_del_fd(pack);
            break;
        case FD_LIST:
            deal_fd_list(pack);
            break;
        case CHAT_FD:
            deal_chat_fd(pack);
            break;
        case FD_CHATSTORE:
            deal_fd_chatstore(pack);
            break;
        case CREAT_GP:
            deal_creat_gp(pack);
            break;
        case JOIN_GP:
            deal_join_gp(pack);
            break;
        case QUIT_GP:
            deal_quit_gp(pack);
            break;
        case CHAT_GP:
            deal_chat_gp(pack);
            break;
        case GP_LIST:
            deal_gp_list(pack);
            break;
        case GP_USER_LIST:
            deal_gp_user_list(pack);
            break;
        case GP_CHATSTORE:
            deal_gp_chat_store(pack);
            break;
        case SEND_FILE:
            deal_send_file(pack);
            break;
        case ONLINE_FD_LIST:
            deal_online_fd_list(pack);
            break;
        case UNTALK_GP:
            deal_untalk_gp(pack);
            break;
        case DEAL_UNTALK_GP:
            deal1_untalk_gp(pack);
            break;
        case INVITE_USER:
            deal_invite_user(pack);
            break;
        case EXIT_GP:
            deal_exit_gp(pack);
            break;
        case BLACK_FD:
            deal_black_fd(pack);
            break;
        case EXIT_BLACK_FD:
            deal_exit_black_fd(pack);
            break;
    }
}

void send_PACK(PACK pack)
{
    int ret;
    if((ret = send(cli_fd, &pack, sizeof(PACK), 0))<0) {
        perror("send_pack：send\n");
    }
}

void send_other_PACK(PACK pack)
{
    int ret;
    if((ret = send(pack.fd, &pack, sizeof(PACK), 0))<0) {
        perror("send_other_pack：send\n");
    }
}

void deal_regist(PACK pack)
{
    PACK send_pack;
    int flag = MYSQL_regist(pack);
    if(flag != -1) {
        send_pack.type = 0;
        printf("客户端%s注册成功\n", pack.username);
        strcpy(send_pack.username, pack.username);
    } else {
        send_pack.type = -1;
    }
    send_PACK(send_pack);
}

typedef struct find_fd {
    char name[USERNAME_LEN];
    int fd;
    struct find_fd *next;
} find_fd;

find_fd *phead = NULL, *pend;

typedef struct online_mess {
    int fd;
    int type;
    char name1[USERNAME_LEN];
    char name2[USERNAME_LEN];
    char name3[USERNAME_LEN];
    char mess[USERNAME_LEN];
    struct online_mess *next;
} online_mess;


online_mess *phead2 = NULL, *pend2, *pnew2;

// 将未上线的用户的消息存在服务端
void deal_online_mess(PACK pack)
{
    if(phead2 == NULL) {
        phead2 = (online_mess *)malloc(sizeof(online_mess));
        phead2->next = NULL;
        pend2 = phead2;
    }
    pnew2 = (online_mess *)malloc(sizeof(online_mess));
    strcpy(pnew2->name1, pack.username);
    //printf("@@@@%s\n", pnew2->name1);
    strcpy(pnew2->name2, pack.send_username);
    //printf("@@@@%s\n", pnew2->name2);
    strcpy(pnew2->name3, pack.password);
    //printf("@@@@%s\n", pnew2->name3);
    strcpy(pnew2->mess, pack.mess);
    //printf("@@@@%s\n", pnew2->mess);
    pnew2->type = pack.type;
    pnew2->next = NULL;
    pend2->next = pnew2;
    pend2 = pnew2;
}
 
// 检测用户登录，发送离线消息盒子的消息 
void send_online_mess(PACK pack)
{
    online_mess *ptemp2;
    ptemp2 = phead2;
    while(ptemp2 != NULL) {
        if(strcmp(pack.username, ptemp2->name2) == 0) {
            //printf("%%%%^^^^^^&&&&&&&#######\n");
            PACK send_pack;
            strcpy(send_pack.username, ptemp2->name1);
            strcpy(send_pack.mess, ptemp2->mess);
            strcpy(send_pack.send_username, ptemp2->name3);
            send_pack.fd = pack.fd;
            send_pack.type = ptemp2->type;
            send_other_PACK(send_pack);
        }
        ptemp2 = ptemp2->next;
    }
}


void deal_login(PACK pack, int cli_fd)
{
    PACK send_pack;
    int flag = MYSQL_login(pack );
    if(flag != -1) {
        send_pack.type = 0;
        printf("客户端%s登陆成功\n", pack.username);
        strcpy(send_pack.username, pack.username);
        if(phead == NULL) {
            phead = (struct find_fd *)malloc(sizeof(struct find_fd));
            phead->next = NULL;
            pend = phead;
        } 
        find_fd *pnew;
        pnew = (struct find_fd *)malloc(sizeof(struct find_fd));
        pnew->fd = cli_fd;
        strcpy(pnew->name, send_pack.username);
        pnew->next = NULL;
        pend->next = pnew;
        pend = pnew;
    } else {
        send_pack.type = -1;
    }
    send_PACK(send_pack);
    pack.fd = cli_fd;
    send_online_mess(pack);
}

void deal_exit(PACK pack)
{
    find_fd *ptemp, *p;
    p = phead;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if (strcmp(ptemp->name, pack.username) == 0) {
            p->next = p->next->next;
            free(ptemp);
            break;
        }
        p = p->next;
        ptemp = ptemp->next;
    }
    MYSQL_exit(pack);
    printf("客户端%s退出成功\n", pack.username);
}

void deal_addfd(PACK pack)
{
    PACK send_pack, send_pack1;
    printf("pack.username = %s\n", pack.username);
    printf("pack.send_username = %s\n", pack.send_username);
    strcpy(send_pack.username, pack.username);
    strcpy(send_pack.send_username, pack.send_username);
    send_pack.type = RECV_ADDFD;
    int oo = 0;
    find_fd *ptemp;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, send_pack.send_username) ==0 ) {
            send_pack.fd = ptemp->fd;
            strcpy(send_pack.username, pack.username);
            oo++;
        }
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack1.fd = ptemp->fd;
            oo++;
        }
        if(oo == 2) {
            break;
        }
        ptemp = ptemp->next;
    }
    int t = MYSQL_deal_addfd(pack);
    if(t == 0) {
        if(ptemp == NULL) {
            deal_online_mess(send_pack);
        }
        else {
            send_other_PACK(send_pack);
        }
    } 
    if(t == -1) {
        send_pack1.type = DEAL_ADDFD;
        strcpy(send_pack1.mess, "fail");
        send_other_PACK(send_pack1);
    }
    //printf("send_pack.send_username = %s\n", send_pack.send_username);
}

void deal_recv_addfd(PACK pack)
{
    PACK send_pack;
    strcpy(send_pack.send_username, pack.send_username);
    strcpy(send_pack.mess, pack.mess);
    printf("send_pack.mess = %s\n", send_pack.mess);
    find_fd *ptemp;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if(strcmp(send_pack.send_username, ptemp->name) == 0) {
            if(strcmp(send_pack.mess, "success") == 0) {
                int t = MYSQL_addfd(pack);
                if(t != -1) {
                    printf("%s %s添加好友成功 写入数据库成功\n", pack.username, pack.send_username);
                }
            } else {
                printf("%s %s添加好友失败\n", pack.username, pack.send_username);
            }
            send_pack.fd = ptemp->fd;
            strcpy(send_pack.username, pack.username);
            break;
        }
        ptemp = ptemp->next;
    }
    send_pack.type = AGREE_ADDFD;
    if(ptemp == NULL) {
        deal_online_mess(send_pack);
    }
    else {
        send_other_PACK(send_pack);
    }
}

void deal_del_fd(PACK pack)
{
    int flag = MYSQL_delfd(pack);
    if(flag == 0) {
        strcpy(pack.mess, "success");
    } else {
        strcpy(pack.mess, "fail");
    }
    pack.type = IS_DEL_FD;
    send_PACK(pack);
}

void deal_fd_list(PACK pack)
{
    PACK send_pack;
    FD_list fd_list;
    int i;
    send_pack.type = RECV_FD_LIST;
    strcpy(fd_list.username, pack.username);
    find_fd *ptemp;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack.fd = ptemp->fd;
            break;
        }
        ptemp = ptemp->next;
    }
    fd_list  = MYSQL_fd_list(pack, fd_list);
   // printf("22222222\n");
    for(i = 0; strcmp(fd_list.send_list[i], "bye") != 0; i++) {
        //printf("%s\n", fd_list.send_list[i]);
        strcpy(send_pack.send_username, fd_list.send_list[i]);
        send_other_PACK(send_pack);
    }
    //printf("fd_list.send_list[%d] = %s\n", i, fd_list.send_list[i]);
    memset(&fd_list, 0, sizeof(FD_list));
}

void deal_online_fd_list(PACK pack)
{
    int t = 1;
    PACK send_pack;
    FD_list fd_list, fd_list1;
    send_pack.type = RECV_ONLINE_FD_LIST;
    strcpy(fd_list.username, pack.username);
    find_fd *ptemp;
    ptemp = phead->next;
     while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack.fd = ptemp->fd;
            break;
        }
        ptemp = ptemp->next;
    }
    fd_list = MYSQL_fd_list(pack, fd_list);
    int i = 0;
    fd_list1 = MYSQL_online_fd_list(fd_list);
    for(int i = 0; strcmp("bye", fd_list1.send_list[i]) != 0; i++) {
        strcpy(send_pack.send_username, fd_list1.send_list[i]);
        send_other_PACK(send_pack);
    }
    memset(&fd_list1, 0, sizeof(FD_list));
}

typedef struct Black_fd {
    char username[USERNAME_LEN];
    char send_username[USERNAME_LEN];
    char mess[50];
    struct Black_fd *next;
} Black_fd;
Black_fd  *phead1 = NULL, *pend1;

void deal_chat_fd(PACK pack)
{
    PACK send_pack, send_pack1;
    strcpy(send_pack.username, pack.username);
    strcpy(send_pack.send_username, pack.send_username);
    strcpy(send_pack.mess, pack.mess);
    send_pack.type = RECV_CHAT_FD;
    find_fd *ptemp;
    ptemp = phead->next;
    int oo = 0;;
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.send_username) == 0) {
            send_pack.fd = ptemp->fd;
            oo++;
        }
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack1.fd = ptemp->fd;
            oo++;
        }
        if(oo == 2) {
            break;
        }
        ptemp = ptemp->next;
    }
    printf("oo = %d\n", oo);
    int t = MYSQL_deal_chat_fd(pack);
    // 处理屏蔽
    if(t == -3) {
        if(phead1 == NULL) {
            phead1 = (struct Black_fd *)malloc(sizeof(struct Black_fd));
            phead1->next = NULL;
            pend1 = phead1;
        } 
        Black_fd *pnew1;
        pnew1 = (struct Black_fd *)malloc(sizeof(struct Black_fd));
        strcpy(pnew1->username, pack.username);
        strcpy(pnew1->send_username, pack.send_username);
        strcpy(pnew1->mess, pack.mess);
        //printf("@@@@@@@@@pack.mess = %s\n", pack.mess);
        pnew1->next = NULL;
        pend1->next = pnew1; 
        pend1 = pnew1;
    }
    if(t == 2) {
        if(ptemp == NULL) {
            printf("用户%s未上线，消息将存在服务端\n", pack.send_username);
            deal_online_mess(send_pack);
        }
        else {
            send_other_PACK(send_pack);
        }
    }
    if(t == 0) { 
        send_pack1.type = DEAL_CHAT_FD;
        strcpy(send_pack1.mess, "fail");
        send_other_PACK(send_pack1);
    }
    MYSQL_chat_fd(send_pack);
    //send_other_PACK(send_pack);
}

void deal_fd_chatstore(PACK pack)
{
    int i = 0;
    PACK send_pack;
    FD_chat fd_chat;
    find_fd *ptemp;
    ptemp = phead->next;
    strcpy(send_pack.username, pack.username);
    strcpy(send_pack.send_username, pack.send_username);
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack.fd = ptemp->fd;
            break;
        }
        ptemp = ptemp->next;
    }
    fd_chat = MYSQL_fd_chatstore(send_pack, fd_chat);
    if(fd_chat.oo != 2) {
        send_pack.type = RECV_FD_CHATSTORE;
        strcpy(send_pack.mess, "fail");
        send_other_PACK(send_pack);
        return;
    }
    while(strcmp(fd_chat.send_username[i], "bye") != 0) {
        send_pack.type = RECV_FD_CHATSTORE;
        strcpy(send_pack.username, fd_chat.username[i]);
        strcpy(send_pack.send_username, fd_chat.send_username[i]);
        strcpy(send_pack.mess, fd_chat.message[i]);
        i++;
        send_other_PACK(send_pack);
    }
    memset(&fd_chat, 0, sizeof(FD_chat));
}


void deal_creat_gp(PACK pack) 
{
    PACK send_pack;
    strcpy(send_pack.username, pack.username);
    strcpy(send_pack.send_username, pack.send_username);
    int flag = MYSQL_creat_gp(pack);
    if(flag == 0) {
        strcpy(send_pack.mess, "success");
    }
    if(flag == -1) {
        strcpy(send_pack.mess, "fail");
    }
    send_pack.type = RECV_CREAT_GP;
    send_PACK(send_pack);
}

void deal_join_gp(PACK pack) 
{
    PACK send_pack;
    send_pack.type = RECV_JOIN_GP;
    strcpy(send_pack.send_username, pack.send_username);
    strcpy(send_pack.username, pack.username);
    int flag = MYSQL_join_gp(send_pack);
    if(flag != 0) {
        strcpy(send_pack.mess, "success");
    } else {
        strcpy(send_pack.mess, "fail");
    }
    send_PACK(send_pack);
}

void deal_quit_gp(PACK pack)
{
    int i = 0;
    PACK send_pack, send_pack1;
    send_pack1.type = RECV_QUIT1_GP;
    QUIT_gp quit_gp;
    send_pack.type = RECV_QUIT_GP;
    //strcpy(send_pack1.send_username, pack.send_username);
    //strcpy(send_pack1.username, pack.username);
    strcpy(send_pack1.username, pack.username);
    strcpy(send_pack.username, pack.username);
    strcpy(send_pack.send_username, pack.send_username);
    quit_gp = MYSQL_quit_gp(send_pack, quit_gp);
    if(quit_gp.flag != 0) {
        strcpy(send_pack.mess, "success");
        if(quit_gp.oo == -1) {
            find_fd *ptemp;
            ptemp = phead->next;
            while(strcmp(quit_gp.username[i], "bye") != 0) {
                ptemp = phead->next;
                while(ptemp != NULL) {
                    if(strcmp(quit_gp.username[i], pack.username) == 0) {
                    break;
                    }
                    if(strcmp(ptemp->name, quit_gp.username[i]) == 0) {
                        strcpy(send_pack1.mess, pack.send_username);
                        send_pack1.fd = ptemp->fd;
                        send_other_PACK(send_pack1);
                        ptemp = phead->next;
                        break;
                    }
                    ptemp = ptemp->next;
                }
                if(ptemp == NULL) {
                    strcpy(send_pack1.send_username, quit_gp.username[i]);
                    strcpy(send_pack1.mess, pack.send_username);
                    deal_online_mess(send_pack1);
                }
                i++;
            }
        }
    } else {
        strcpy(send_pack.mess, "fail");
    }
    send_PACK(send_pack);
}

void deal_chat_gp(PACK pack)
{
    int i = 0;
    PACK send_pack;
    find_fd *ptemp;
    ptemp = phead->next;
    int flag = MYSQL_deal_chat_gp(pack);
    //printf("flag = %d\n", flag);
    //printf("@@@@@@@@@@\n");
    if(flag == -1) {
        // 处理禁言
        while(ptemp != NULL) {
            if(strcmp(ptemp->name, pack.username) == 0) {
                PACK pack1;
                strcpy(pack1.mess, "fail");
                pack1.type = FAIL_CHAT_GP;
                pack1.fd = ptemp->fd;
                //printf("********\n");
                send_other_PACK(pack1);
                return;
            }
            ptemp = ptemp->next;
        }
    }
    chat_GP chat_gp;
    send_pack.type = RECV_CHAT_GP;
    strcpy(send_pack.username, pack.username);
    strcpy(send_pack.send_username, pack.send_username);
    strcpy(send_pack.mess, pack.mess);
    chat_gp = MYSQL_chat_gp(pack, chat_gp);
    if(chat_gp.oo != 2) {
        PACK send_pack1;
        send_pack1.type = RECV_CHAT_GP1;
        strcpy(send_pack1.mess, "fail");
        //send_pack1.fd = pack1.fd;
        send_PACK(send_pack1);
        return;
    }
    while(strcmp("bye", chat_gp.send_username[i]) != 0) {
        while(ptemp != NULL) {
            if(strcmp(chat_gp.send_username[i], pack.username) == 0) {
                break;
            }
            if(strcmp(ptemp->name, chat_gp.send_username[i]) == 0) {
                send_pack.fd = ptemp->fd;
                send_other_PACK(send_pack);
                ptemp = phead->next;
                break;
            }
            ptemp = ptemp->next;
        }
        if(ptemp == NULL) {
            PACK pack2;
            pack2.type = RECV_CHAT_GP;
            strcpy(pack2.username, pack.username);
            printf("用户%s未上线\n", chat_gp.send_username[i]);
            strcpy(pack2.send_username, chat_gp.send_username[i]);
            strcpy(pack2.mess, pack.mess);
            strcpy(pack2.password, pack.send_username);
            deal_online_mess(pack2);
        }
        i++;
    }
}


void deal_gp_list(PACK pack)
{
    int i = 0;
    PACK send_pack;
    send_pack.type = RECV_GP_LIST;
    GP_list gp_list;
    gp_list = MYSQL_gp_list(pack, gp_list);
    find_fd *ptemp;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack.fd  = ptemp->fd;
            break;
        }
        ptemp = ptemp->next;
    }
    while(strcmp(gp_list.list[i], "bye") != 0) {
        strcpy(send_pack.send_username, gp_list.list[i]);
        send_other_PACK(send_pack);
        i++;
    }
}

void deal_gp_user_list(PACK pack) 
{
    int i = 0;
    int t = 1;
    PACK send_pack;
    send_pack.type = RECV_GP_USER_LIST;
    strcpy(send_pack.username, pack.send_username);
    GP_user_list gp_user_list;
    gp_user_list = MYSQL_gp_user_list(pack, gp_user_list);
    find_fd *ptemp;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack.fd = ptemp->fd;
        }
        ptemp = ptemp->next;
    }
    if(gp_user_list.oo == -1) {
        strcpy(send_pack.mess, "fail");
        send_other_PACK(send_pack);
        return;
    }
    if(gp_user_list.oo == 2) {
        strcpy(send_pack.mess, "success");
    }
    while(strcmp(gp_user_list.list[i], "bye") != 0) {
        strcpy(send_pack.send_username, gp_user_list.list[i]);
        send_other_PACK(send_pack);
        i++;
    }
}

void deal_gp_chat_store(PACK pack)
{
    int i;
    int t = 1;
    PACK send_pack;
    find_fd *ptemp;
    ptemp = phead->next;
   // strcpy(send_pack.username, pack.username);
    //strcpy(send_pack.send_username, pack.send_username);
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack.fd = ptemp->fd;
            break;
        }
        ptemp = ptemp->next;
    }
    GP_chatstore gp_chatstore;
    gp_chatstore = MYSQL_gp_chatstore(pack, gp_chatstore);
    if(gp_chatstore.oo != 2) {
        send_pack.type = RECV_GP_CHATSTORE;
        strcpy(send_pack.mess, "fail");
        send_other_PACK(send_pack);
        return;
    }
    while(strcmp(gp_chatstore.username[i], "bye") != 0) {
        send_pack.type = RECV_GP_CHATSTORE;
        strcpy(send_pack.username, gp_chatstore.username[i]);
        //strcpy(send_pack.send_username, gp_chatstore.send_username[i]);
        strcpy(send_pack.mess, gp_chatstore.mess[i]);
        i++;
        send_other_PACK(send_pack);
    }
    memset(&gp_chatstore, 0, sizeof(GP_chatstore));
}

void deal_untalk_gp(PACK pack)
{
    pack.type = RECV_UNTALK_GP;
    find_fd *ptemp;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if(strcmp(pack.username, ptemp->name) == 0) {
            pack.fd = ptemp->fd;
            break;
        }
        ptemp = ptemp->next;
    }
    int flag = MYSQL_untalk_gp(pack);
    if(flag != 2) {
        strcpy(pack.password, "no");
        send_other_PACK(pack);
    }
    if(flag == 2) {
        strcpy(pack.password, "yes");
        int oo = MYSQL_deal_untalk_gp(pack);
        send_other_PACK(pack);
    }
}

void deal1_untalk_gp(PACK pack)
{
    pack.type = RECV_DEAL_UNTALK_GP;
     find_fd *ptemp;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if(strcmp(pack.username, ptemp->name) == 0) {
            pack.fd = ptemp->fd;
            break;
        }
        ptemp = ptemp->next;
    }
    int flag = MYSQL_untalk1_gp(pack);
    if(flag != 2) {
        strcpy(pack.password, "no");
        send_other_PACK(pack);
    }
    if(flag == 2) {
        strcpy(pack.password, "yes");
        MYSQL_deal1_untalk_gp(pack);
        send_other_PACK(pack);
    }
}

void deal_invite_user(PACK pack) 
{
    PACK send_pack, send_pack1;
    send_pack.type = RECV_INVITE_USER;
    strcpy(send_pack.username, pack.username);
    strcpy(send_pack.send_username, pack.send_username);
    strcpy(send_pack.mess, pack.mess);
    send_pack1.type = IS_INVITE_USER;
    find_fd *ptemp;
    ptemp = phead->next;
    int oo = 0;
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.send_username) ==0 ) {
            send_pack.fd = ptemp->fd;
            oo++;
        }
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack1.fd = ptemp->fd;
            oo++;
        }
        if(oo == 2) {
            break;
        }
        ptemp = ptemp->next;
    }
    int flag = MYSQL_deal_invite_user(pack);
    if(flag == 3) {
        if(ptemp == NULL) {
            deal_online_mess(send_pack);
        } else {
            send_other_PACK(send_pack);
        }
        //send_other_PACK(send_pack);
        MYSQL_invite_user_isok(pack);
        strcpy(send_pack1.password, "success");
        send_other_PACK(send_pack1);
    }
    if(flag != 3) {
        strcpy(send_pack1.password, "fail");
        send_other_PACK(send_pack1);
    }
}

void deal_exit_gp(PACK pack) 
{
    
    PACK send_pack,send_pack1;
    send_pack.type = RECV_EXIT_GP;
    strcpy(send_pack.username, pack.username);
    strcpy(send_pack.send_username, pack.send_username);
    strcpy(send_pack.mess, pack.mess);
    send_pack1.type = IS_EXIT_GP;
    find_fd *ptemp;
    ptemp = phead->next;
    int oo = 0;
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.send_username) ==0 ) {
            send_pack.fd = ptemp->fd;
            oo++;
        }
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack1.fd = ptemp->fd;
            oo++;
        }
        if(oo == 2) {
            break;
        }
        ptemp = ptemp->next;
    }
    int flag = MYSQL_deal_exit_gp(pack);
    if(flag == 2) {
        if(ptemp == NULL) {
            deal_online_mess(send_pack);
        }
        //send_other_PACK(send_pack);
        MYSQL_exit_gp_isok(pack);
        strcpy(send_pack1.password, "success");
        send_other_PACK(send_pack1);
    } else {
        strcpy(send_pack1.password, "fail");
        send_other_PACK(send_pack1);
    }
}

void deal_black_fd(PACK pack)
{
    PACK send_pack;
    pack.type = IS_BLACK_FD;
    find_fd *ptemp;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.username) == 0) {
            pack.fd = ptemp->fd;
            break;
        }
        ptemp = ptemp->next;
    }
    int t =  MYSQL_black_fd(pack);
    if(t == 2) {
        strcpy(pack.mess, "success");
    } else {
        strcpy(pack.mess, "fail");
    }
    send_other_PACK(pack);
}

void deal_exit_black_fd(PACK pack)
{
    PACK send_pack;
    Black_fd *ptemp1;
    ptemp1 = phead1;
    find_fd *ptemp;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if(strcmp(ptemp->name, pack.username) == 0) {
            send_pack.fd = ptemp->fd;
            break;
        }
        ptemp = ptemp->next;
    }
    send_pack.type = IS_EXIT_BLACK_FD;
    //send_other_PACK(send_pack);
    int t = MYSQL_exit_black_fd(pack);
    if(t == 0) {
        //printf("%%%%%%%%\n");
        strcpy(send_pack.mess, "fail");
        send_other_PACK(send_pack);
        return;
    }
    strcpy(send_pack.mess, "success");
    //printf("#############解除屏蔽成功\n");
    send_other_PACK(send_pack);
    while(ptemp1 != NULL) {
        if(strcmp(ptemp1->username, pack.send_username) == 0 && strcmp(ptemp1->send_username, pack.username) == 0) {
            send_pack.type = RECV_CHAT_FD;
            strcpy(send_pack.username, ptemp1->username);
            strcpy(send_pack.mess, ptemp1->mess);
            send_other_PACK(send_pack);
        }
        ptemp1 = ptemp1->next;
    }
}

void deal_send_file(PACK pack)
{
    find_fd *ptemp;
    ptemp = phead->next;
    while(ptemp != NULL) {
        if(strcmp(pack.send_username, ptemp->name) == 0) {
            pack.fd = ptemp->fd;
            break;
        }
        ptemp = ptemp->next;
    }
    pack.type = RECV_FILE;
    send_other_PACK(pack);
}

