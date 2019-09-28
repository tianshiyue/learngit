/*************************************************************************
	> File Name: MYSQL.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月09日 星期一 16时14分03秒
 ************************************************************************/
#include "define.h"

#include <stdio.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>

int                 i;
int                 ret;
unsigned int        num_fields;
MYSQL               mysql;
MYSQL_RES           *result = NULL;
MYSQL_ROW           row;
MYSQL_FIELD         *field;

void  accept_mysql()
{
	if(NULL == mysql_init(&mysql)){
		perror("mysql_init\n");
	}

	// 初始化数据库
	if(mysql_library_init(0, NULL, NULL) != 0){
		perror("mysql_library_init\n");
	}

	// 连接数据库
	if(NULL == mysql_real_connect(&mysql, "127.0.0.1", "root", "102089wy", "care", 0, NULL, 0)){
		perror("mysql_real_connect\n");
	}

	// 设置中文字符集
	if(mysql_set_character_set(&mysql, "utf8") < 0){
		perror("mysql_set_character_set\n");
	}

	printf("连接mysql数据库成功\n");
}
void  Init_mysql() {
    accept_mysql();
    if(mysql_query(&mysql, "update user set online = 0" )){
        perror("Init_mysql:mysql_query\n");
    }
    mysql_close(&mysql);
}
int  MYSQL_regist(PACK pack)
{
    accept_mysql();
    int flag = 0;
    mysql_query(&mysql, "select *from user");
    result = mysql_store_result(&mysql);
    if (result){
        while((row = mysql_fetch_row(result))) {
            // 查找用户名是否重复
            if (strcmp(row[0], pack.username) == 0) {
                flag = -1;
            }
        }
    }
    if (flag == 0) {
        char buff[200];
        printf("输入成功\n");
        sprintf(buff, "insert into user(username, password) values ('%s', '%s')", pack.username, pack.password);
        int ret = mysql_query(&mysql, buff);
        if (ret) {
            perror("MYSQL_regist: mysql_query\n");
        }
    }
    mysql_close(&mysql);
    return flag;
}

int  MYSQL_login(PACK pack ) 
{
    accept_mysql();
    int flag = -1;
    char buff[200];
    mysql_query(&mysql, "select *from user");
    result = mysql_store_result(&mysql);
    if (result) {
        while((row = mysql_fetch_row(result))) {
            if ((strcmp(row[0], pack.username) == 0) && (strcmp(row[1], pack.password) == 0)) {
                sprintf(buff, "update user set online = 1 where username = '%s'", pack.username);
                mysql_query(&mysql, buff);
                flag = 0;
                break;
            }
        }
    }
    return flag;
}

int MYSQL_deal_addfd(PACK pack)
{
    accept_mysql();
    int flag = 0;
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if((strcmp(pack.username, row[0]) == 0 && strcmp(pack.send_username, row[1]) == 0) || (strcmp(pack.send_username, row[0]) == 0 && strcmp(pack.username, row[1]) == 0)) {
                flag = -1;
                return flag;
            }
        }
    }
    return flag;
}

void MYSQL_exit(PACK pack)
{
    char buff[200];
    sprintf(buff, "update user set online = 0 where username = '%s'", pack.username);
    mysql_query(&mysql, buff);
}

int  MYSQL_addfd(PACK pack)
{
    accept_mysql();
    int flag = 0;
    char buff[200];
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    sprintf(buff, "insert into friends (username1, username2, flag)values ('%s', '%s', 1)", pack.username, pack.send_username);
    ret = mysql_query(&mysql,buff);
    if (!ret) {
        sprintf(buff, "insert into friends (username1, username2, flag)values ('%s', '%s', 1)", pack.send_username, pack.username);
        ret = mysql_query(&mysql,buff);
        if (!ret) {
            mysql_close(&mysql);
            return flag;
        }
    }
}

int  MYSQL_delfd(PACK pack)
{
    accept_mysql();
    int flag = -1;
    char buff[200];
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(pack.username, row[0]) == 0) {
                if(strcmp(pack.send_username, row[1]) == 0) {
                    sprintf(buff,"delete from friends where (username1 = '%s' and username2 = '%s') or (username1 ='%s' and username2 = '%s')", pack.username, pack.send_username, pack.send_username, pack.username);
                    mysql_query(&mysql, buff);
                    flag = 0;
                    return flag;
                }
            }
        }
    }
    return flag;
}

int MYSQL_deal_chat_fd(PACK pack)
{
    accept_mysql();
    int flag = 0;
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            // A给B发消息 B可能屏蔽A
            // flag = 2：A与B为好友 
            if(strcmp(pack.username, row[0]) == 0 && strcmp(pack.send_username, row[1]) == 0 && atoi(row[2]) == 1) {
                //printf("**********\n");
                flag ++;
            }
            if(strcmp(pack.username, row[1]) == 0 && strcmp(pack.send_username, row[0]) == 0 && atoi(row[2]) == 1) {
                //printf("###########\n");
                flag++;
            }
            // B屏蔽A
            if(strcmp(pack.username, row[1]) == 0 && strcmp(pack.send_username, row[0]) == 0 && atoi(row[2]) == 0) {
                //printf("@@@@@@@@@\n");
                flag = -3;
                break;
            }
        }
    }
    printf("flag = %d\n", flag);
    return flag;
}

void MYSQL_chat_fd(PACK pack)
{
    accept_mysql();
    char buff[200];
    sprintf(buff, "insert into fd_chatstore (username, send_username, mess, time) values ('%s', '%s', '%s', '%s')", pack.username, pack.send_username, pack.mess, pack.password);
    ret = mysql_query(&mysql,buff);
}

FD_list  MYSQL_fd_list(PACK pack, FD_list fd_list)
{
    accept_mysql();
    char buff[200];
    int i = 0;
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    printf("pack.username = %s\n", pack.username);
    memset(&fd_list, 0, sizeof(FD_list));
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[0], pack.username) == 0) {
                //printf("row[1] = %s\n", row[1]);
                strcpy(fd_list.send_list[i], row[1]);
                //printf("fd_list.send_list[%d] = %s\n", i, fd_list.send_list);
                i++;
            }
        }
    }
    strcpy(fd_list.send_list[i], "bye");
    //printf("fd_list.send_list[%d] = %s\n", i, fd_list.send_list);
    return fd_list;
}

FD_list MYSQL_online_fd_list(FD_list fd_list)
{
    accept_mysql();
    int i = 0;
    int k = 0;
    FD_list fd_list1;
    memset(&fd_list1, 0, sizeof(FD_list));
    while(strcmp("bye", fd_list.send_list[i]) != 0) {
        mysql_query(&mysql, "select *from user");
        result = mysql_store_result(&mysql);
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[0], fd_list.send_list[i]) == 0 && atoi(row[2]) == 1) {
                strcpy(fd_list1.send_list[k], row[0]);
                k++;
                break;
            }
        }
        i++;
    }
    strcpy(fd_list1.send_list[k], "bye");
    return fd_list1;
}

FD_chat MYSQL_fd_chatstore(PACK pack, FD_chat fd_chat)
{
    accept_mysql();
    char buff[200];
    int i = 0;
    fd_chat.oo = 0;
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(pack.username, row[0]) == 0 && strcmp(pack.send_username, row[1]) == 0) {
                fd_chat.oo += 1;
            }
            if(strcmp(pack.username, row[1]) == 0 && strcmp(pack.send_username, row[0]) == 0) {
                fd_chat.oo += 1;
            }
        }
    }
    int t = fd_chat.oo;
    mysql_query(&mysql, "select *from fd_chatstore");
    result = mysql_store_result(&mysql);
    memset(&fd_chat, 0, sizeof(FD_chat));
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if((strcmp(row[0], pack.username) == 0 && strcmp(row[1], pack.send_username) == 0)) {
                strcpy(fd_chat.username[i], row[0]);
                strcpy(fd_chat.send_username[i], row[1]);
                strcpy(fd_chat.message[i], row[2]);
                strcpy(fd_chat.time[i], row[3]);
                i++;
            }
            if(strcmp(row[0], pack.send_username) == 0 && strcmp(row[1], pack.username) == 0) {
                strcpy(fd_chat.username[i], row[0]);
                strcpy(fd_chat.send_username[i], row[1]);
                strcpy(fd_chat.message[i], row[2]);
                strcpy(fd_chat.time[i], row[3]);
                i++;
            }
        }
    }
    strcpy(fd_chat.send_username[i], "bye");
    fd_chat.oo = t;
    return fd_chat;
}


int  MYSQL_creat_gp(PACK pack)
{
    accept_mysql();
    int flag = 0;
    char buff[200];
    mysql_query(&mysql, "select *from groups");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[1], pack.send_username) == 0) {
                flag = -1;
                return flag;
            }
        }
    }
    sprintf(buff, "insert into groups(group_own, group_name) values ('%s','%s')", pack.username, pack.send_username);
    ret = mysql_query(&mysql, buff);
    sprintf(buff, "insert into group_member (group_name, group_user, flag) values ('%s', '%s', 1)", pack.send_username, pack.username);
    ret = mysql_query(&mysql, buff);
    printf("群%s创建成功\n", pack.send_username);
    return flag;
}

int MYSQL_join_gp(PACK pack)
{
    int flag = 0;
    int i = 0;
    accept_mysql();
    char buff[200];
    mysql_query(&mysql, "select *from groups");
    result = mysql_store_result(&mysql);
    if (result) {
        while((row = mysql_fetch_row(result))) {
            if (strcmp(row[1], pack.send_username) == 0) {
                sprintf(buff, "insert into group_member (group_name, group_user, flag) values ('%s', '%s', 0)", pack.send_username, pack.username);
                ret = mysql_query(&mysql, buff);
                flag = 1;
                break;
            }
            i++;
        }
    }
    return flag;
}

QUIT_gp  MYSQL_quit_gp(PACK pack, QUIT_gp quit_gp) 
{
    int  i = 0;
    quit_gp.flag = 0;
    quit_gp.oo = 0;
    char buff[200];
    accept_mysql();
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    if(result) {
        while((row = mysql_fetch_row(result))) {
            if((strcmp(row[0], pack.send_username) ==0 && strcmp(row[1], pack.username) == 0)) {
                quit_gp.flag = 1;
                if(atoi(row[2]) == 1) {
                    quit_gp.oo = -1;
                }
                mysql_query(&mysql, "select *from group_member");
                result = mysql_store_result(&mysql);
                while(row = mysql_fetch_row(result)) {
                    if(strcmp(row[0], pack.send_username) == 0) {
                        strcpy(quit_gp.username[i], row[1]);
                        i++;
                    }
                }
                break;
            }
        }
    }
    strcpy(quit_gp.username[i], "bye");
    int j = 0;
    // 群主退群
    if(quit_gp.oo == -1) {
        mysql_query(&mysql, "select *from group_member");
        result = mysql_store_result(&mysql);
        while(strcmp(quit_gp.username[j], "bye") != 0) {
            sprintf(buff,"delete from group_member where (group_name = '%s' and group_user = '%s')", pack.send_username, quit_gp.username[j]);
            mysql_query(&mysql, buff);
            j++;
        }
            sprintf(buff,"delete from groups where (group_own = '%s')", pack.username);
            mysql_query(&mysql, buff);
    }
    return quit_gp;
}

int MYSQL_deal_chat_gp(PACK pack) 
{
    int flag = 0;
    accept_mysql();
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    // 处理禁言
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(pack.username, row[1]) == 0 && strcmp(pack.send_username, row[0]) == 0) {
                if(atoi(row[2]) == -1) {
                    flag = -1;
                    return flag;
                }
            }
        }
    }
    return flag;
}

chat_GP MYSQL_chat_gp(PACK pack, chat_GP chat_gp)
{
    accept_mysql();
    char buff[200];
    int i = 0;
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    memset(&chat_gp, 0, sizeof(chat_GP));
    chat_gp.oo = 0;
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[1], pack.username) == 0 && atoi(row[2]) == -1) {
                break;
            }
            if(strcmp(row[0], pack.send_username) == 0) {
                //printf("chat_gp.send_username[%d] = %s\n", i,  row[1]);
                strcpy(chat_gp.send_username[i], row[1]);
                i++;
                if(strcmp(row[1], pack.username) == 0) {
                    chat_gp.oo = 2;
                }
            }
        }
    }
    strcpy(chat_gp.send_username[i++], "bye");
   // printf("chat_gp.send_usernamep[%d] = %s\n",i , chat_gp.send_username);
    if(chat_gp.oo == 2) {
        sprintf(buff, "insert into gp_chatstore (group_name, group_username, mess, time) values ('%s', '%s', '%s', '%s')", pack.send_username, pack.username, pack.mess, pack.password);
        mysql_query(&mysql,buff);
    }
    return chat_gp;
}

GP_list MYSQL_gp_list(PACK pack, GP_list gp_list)
{
    int i = 0;
    accept_mysql();
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    memset(&gp_list, 0, sizeof(chat_GP));
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(pack.username, row[1]) == 0) {
                strcpy(gp_list.list[i], row[0]);
                //printf("gp_list.list[%d] = %s\n", i, gp_list.list[i]);
                i++;
            }
        }
    }
    strcpy(gp_list.list[i++], "bye");
    return gp_list;
}

GP_user_list MYSQL_gp_user_list(PACK pack, GP_user_list gp_user_list)
{
    int i = 0;
    accept_mysql();
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    memset(&gp_user_list, 0, sizeof(gp_user_list));
    gp_user_list.oo = -1;
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[0], pack.send_username) == 0) {
                strcpy(gp_user_list.list[i], row[1]);
                i++;
                if(strcmp(pack.username, row[1]) == 0) {
                    gp_user_list.oo = 2;
                }
            }
        }
    }
    strcpy(gp_user_list.list[i++], "bye");
    return gp_user_list;
}

GP_chatstore MYSQL_gp_chatstore(PACK pack, GP_chatstore gp_chatstore)
{
    accept_mysql();
    char buff[200];
    int i = 0;
    mysql_query(&mysql, "select *from gp_chatstore");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(pack.send_username, row[0]) == 0) {
                if(strcmp(pack.username, row[1]) == 0) {
                    gp_chatstore.oo = 2;
                }
            }
        }
    }
    int t = gp_chatstore.oo;
    mysql_query(&mysql, "select *from gp_chatstore");
    result = mysql_store_result(&mysql);
    memset(&gp_chatstore, 0, sizeof(GP_chatstore));
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[0], pack.send_username) == 0) {
                strcpy(gp_chatstore.username[i], row[1]);
                strcpy(gp_chatstore.mess[i], row[2]);
                strcpy(gp_chatstore.time[i], row[3]);
                i++;
            }
        }
    }
    strcpy(gp_chatstore.username[i], "bye");
    gp_chatstore.oo = t;
    return gp_chatstore;
}

int MYSQL_untalk_gp(PACK pack)
{
    accept_mysql();
    int flag = 0;
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[0], pack.mess) == 0) {
                //printf("qunqunqun\n");
                if(strcmp(row[1], pack.send_username) == 0 && atoi(row[2]) == 0) {
                    //printf("beijinyanderen\n");
                    flag++;
                } 
                if(strcmp(row[1], pack.username) == 0 && atoi(row[2]) == 1) {
                    //printf("jinyanzhequanxian\n");
                    flag++;
                }
            }
        }
    }
    //printf("#####flag = %d\n", flag);
    return flag;
}

int  MYSQL_deal_untalk_gp(PACK pack)
{
    accept_mysql();
    char buff[200];
    int flag = -1;
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    if(result) {
        printf("pack.send_username = %s\n", pack.send_username);
        while(row = mysql_fetch_row(result)) {
            printf("row[1] = %s\n", row[1]);
            if(strcmp(row[1], pack.send_username) == 0) {
                //printf("^^^^^^^^^^^^^\n");
                sprintf(buff, "update group_member set flag = -1 where group_user = '%s' and group_name = '%s'", pack.send_username, pack.mess);
                //printf("%s\n", buff);
                mysql_query(&mysql, buff);
                flag = 0;
                return flag;
            }
        }
    }
    return flag;
}

int MYSQL_untalk1_gp(PACK pack)
{
    accept_mysql();
    int flag = 0;
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[0], pack.mess) == 0) {
                if(strcmp(row[1], pack.username) == 0 && atoi(row[2]) == 1) {
                    //printf("解除禁言的群主\n");
                    flag++;
                }
                if(strcmp(row[1], pack.send_username) == 0 && atoi(row[2]) == -1) {
                    flag++;
                    //printf("被解除禁言的用户\n");
                }
            }
        }
    }
    return flag;
}

void MYSQL_deal1_untalk_gp(PACK pack)
{
    accept_mysql();
    char buff[200]; 
    sprintf(buff, "update group_member set flag = 0 where group_user = '%s' and group_name = '%s'", pack.send_username, pack.mess);
    mysql_query(&mysql, buff);
}


int MYSQL_deal_invite_user(PACK pack)
{
    accept_mysql();
    int flag = 0;
    char join_user[20][20];
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(pack.username, row[0]) == 0 && strcmp(pack.send_username, row[1]) == 0) {
                flag++;
            }
            if(strcmp(pack.username, row[1]) == 0 && strcmp(pack.send_username, row[0]) == 0) {
                flag++;
            }
        }
    }
    if(flag != 2) {
        return flag;
    }
    //printf("flag为2是好友\n");
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[0], pack.mess) == 0) {
                if(strcmp(row[1], pack.username) == 0) {
                    //printf("主动邀请的人在群里, flag为3\n");
                    flag++;
                }
                if(strcmp(row[1], pack.send_username) == 0) {
                    //printf("被邀请者在群里, flag变为2\n");
                    flag--;
                }
            }
        }
    }
    //printf("flag = %d\n",flag);
    return flag;
}

void MYSQL_invite_user_isok(PACK pack)
{
    accept_mysql();
    char buff[200];
    sprintf(buff, "insert into group_member (group_name, group_user, flag) values ('%s', '%s', 0)", pack.mess, pack.send_username);
    //printf("%s\n", buff);
    mysql_query(&mysql, buff);
}

int MYSQL_deal_exit_gp(PACK pack)
{
    accept_mysql();
    int flag = 0;
    int i = 1;
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[0], pack.mess) == 0) {
                if(strcmp(row[1], pack.send_username) == 0) {
                    flag++;
                }
                if(strcmp(row[1], pack.username) == 0 && atoi(row[2]) == 1) {
                    flag++;
                }
            }
        }
    }
    return flag;
}

void MYSQL_exit_gp_isok(PACK pack)
{
    accept_mysql();
    char buff[200];
    sprintf(buff,"delete from group_member where (group_name = '%s' and group_user = '%s')", pack.mess, pack.send_username);
    mysql_query(&mysql, buff);
}

int MYSQL_black_fd(PACK pack)
{
    accept_mysql();
    char buff[200];
    int flag = 0;
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    while(row = mysql_fetch_row(result)) {
        if(strcmp(pack.username, row[0]) == 0 && strcmp(pack.send_username, row[1]) == 0 && atoi(row[2]) != 0) {
            //printf("%s与%s是好友，并且%s没有将%s拉黑\n", pack.username, pack.send_username, pack.username, pack.send_username);
            flag++;
        }
        if(strcmp(pack.username, row[1]) == 0 && strcmp(pack.send_username, row[0]) == 0) {
            //printf("%s与%s是好友\n", pack.send_username, pack.username);
            flag++;
        }
    }
    if(flag != 2) {
        return flag;
    }
    sprintf(buff, "update friends set flag = 0 where username1 = '%s' and username2 = '%s'", pack.username, pack.send_username);
    mysql_query(&mysql, buff);
    return flag;
}

int  MYSQL_exit_black_fd(PACK pack)
{
    accept_mysql();
    char buff[200];
    int flag = 0;
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    while(row = mysql_fetch_row(result)) {
        if(strcmp(pack.username, row[0]) == 0 && strcmp(pack.send_username, row[1]) == 0 && atoi(row[2]) == 0) {
            flag++;
        }
    }
    if(flag != 1) {
        return flag;
    }
    sprintf(buff, "update friends set flag = 1 where username1 = '%s' and username2 = '%s'", pack.username, pack.send_username); 
    mysql_query(&mysql, buff);
    return flag;
}
