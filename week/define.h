/*************************************************************************
	> File Name: define.h
	> Author: 
	> Mail: 
	> Created Time: 2019年09月08日 星期日 20时25分44秒
 ************************************************************************/

#ifndef _DEFINE_H
#define _DEFINE_H

#define USERNAME_LEN 20
#define PASSWORD_LEN 20
#define INITA      1111
#define INITB      2222

#define EXITA      0
#define EXITB      00

#define REGIST     1
#define LOGIN      2

#define ADD_FD     10
#define DEL_FD     11
#define CHAT_FD    12
#define CREAT_GP   13
#define JOIN_GP    14
#define QUIT_GP    15
#define CHAT_GP    16
#define SEND_FILE  17
#define MESS_RECV_ADDFD 18
#define FD_LIST    19
#define FD_CHATSTORE   20
#define GP_LIST    21
#define GP_USER_LIST 22
#define GP_CHATSTORE 23
#define ONLINE_FD_LIST 24
#define UNTALK_GP    25
#define DEAL_UNTALK_GP 26
#define INVITE_USER 27
#define EXIT_GP     28
#define BLACK_FD    29
#define EXIT_BLACK_FD 30

#define RECV_ADDFD 1001
#define AGREE_ADDFD 1002
#define RECV_CHAT_FD 1003
#define RECV_FD_LIST 1004
#define RECV_FD_CHATSTORE 1005
#define RECV_CREAT_GP 1006
#define RECV_JOIN_GP 1007
#define RECV_QUIT_GP 1008
#define RECV_CHAT_GP 1009
#define RECV_GP_LIST 1010
#define RECV_GP_USER_LIST 1011
#define RECV_GP_CHATSTORE 1012
#define RECV_FILE    1013

#define DEAL_ADDFD   1014
#define DEAL_CHAT_FD 1015
#define RECV_ONLINE_FD_LIST 1016

#define RECV_UNTALK_GP 1017
#define FAIL_CHAT_GP   1018
#define RECV_DEAL_UNTALK_GP 1019
#define RECV_INVITE_USER 1020
#define IS_INVITE_USER   1021
#define RECV_QUIT1_GP    1022
#define RECV_EXIT_GP     1023
#define IS_EXIT_GP       1024
#define IS_BLACK_FD      1025
#define IS_EXIT_BLACK_FD 1026
#define IS_DEL_FD        1027

typedef struct PACK {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    char send_username[USERNAME_LEN];
    char mess[1000];
    int  type;
    int  fd;
} PACK __attribute__((aligned(1)));

typedef struct PACK_FD_LIST {
    int  type;
    char username[USERNAME_LEN];
    char fdlist[USERNAME_LEN][USERNAME_LEN];
} PACK_FD_LIST;

typedef struct FD_list {
    char username[USERNAME_LEN];
    char send_list[100][100];
} FD_list;

typedef struct FD_chat {
    char username[USERNAME_LEN][USERNAME_LEN];
    char send_username[USERNAME_LEN][USERNAME_LEN];
    char message[100][100];
} FD_chat;

typedef struct QUIT_gp {
    char username[30][30];
    int flag;
    int oo;
} QUIT_gp;

typedef struct chat_GP {
    char username[USERNAME_LEN];
    char groupname[USERNAME_LEN];
    char send_username[50][50];
    char message[50];
} chat_GP;

typedef struct GP_list {
    char username[USERNAME_LEN];
    char list[50][50];
} GP_list;

typedef struct GP_user_list {
    char username[USERNAME_LEN];
    char list[50][50];
} GP_user_list;

typedef struct GP_chatstore {
    char username[USERNAME_LEN][USERNAME_LEN];
    char send_username[USERNAME_LEN][USERNAME_LEN];
    char mess[50][50];
} GP_chatstore;

#endif
