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
    mysql_query(&mysql, "select *from user");
    result = mysql_store_result(&mysql);
    if (result) {
        while((row = mysql_fetch_row(result))) {
            if ((strcmp(row[0], pack.username) == 0) && (strcmp(row[1], pack.password) == 0)) {
                mysql_query(&mysql, "update user set online = 1" );
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

int  MYSQL_addfd(PACK pack)
{
    accept_mysql();
    int flag = 0;
    char buff[200];
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    sprintf(buff, "insert into friends (username1, username2)values ('%s', '%s')", pack.username, pack.send_username);
    ret = mysql_query(&mysql,buff);
    if (!ret) {
        sprintf(buff, "insert into friends (username1, username2)values ('%s', '%s')", pack.send_username, pack.username);
        ret = mysql_query(&mysql,buff);
        if (!ret) {
            mysql_close(&mysql);
            return flag;
        }
    }
}

void MYSQL_delfd(PACK pack)
{
    accept_mysql();
    char buff[200];
    sprintf(buff,"delete from friends where (username1 = '%s' and username2 = '%s') or (username1 ='%s' and username2 = '%s')", pack.username, pack.send_username, pack.send_username, pack.username);
    mysql_query(&mysql, buff);
}

int MYSQL_deal_chat_fd(PACK pack)
{
    accept_mysql();
    int flag = -1;
    mysql_query(&mysql, "select *from friends");
    result = mysql_store_result(&mysql);
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if((strcmp(pack.username, row[0]) == 0 && strcmp(pack.send_username, row[1]) == 0) && (strcmp(pack.send_username, row[0]) == 0 && strcmp(pack.username, row[1]) != 0)) {
                flag = 0;
                return flag;
            }
        }
    }
    return flag;
}

void MYSQL_chat_fd(PACK pack)
{
    accept_mysql();
    char buff[200];
    sprintf(buff, "insert into fd_chatstore (username, send_username, mess) values ('%s', '%s', '%s')", pack.username, pack.send_username, pack.mess);
    //printf("%s\n", buff);
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
                printf("row[1] = %s\n", row[1]);
                strcpy(fd_list.send_list[i], row[1]);
                printf("fd_list.send_list[%d] = %s\n", i, fd_list.send_list);
                i++;
            }
        }
    }
    //printf("i = %d\n", i);
    strcpy(fd_list.send_list[i+1], "bye");
    return fd_list;
}

FD_chat MYSQL_fd_chatstore(PACK pack, FD_chat fd_chat)
{
    accept_mysql();
    char buff[200];
    int i = 0;
    mysql_query(&mysql, "select *from fd_chatstore");
    result = mysql_store_result(&mysql);
    memset(&fd_chat, 0, sizeof(FD_chat));
    if(result) {
        while(row = mysql_fetch_row(result)) {
            //printf("pack.username = %s\n", pack.username);
            //printf("pack.send_username = %s\n", pack.send_username);
            if((strcmp(row[0], pack.username) == 0 && strcmp(row[1], pack.send_username) == 0) || (strcmp(row[0], pack.send_username) == 0 && strcmp(row[1], pack.username) == 0)) {
                strcpy(fd_chat.username[i], row[0]);
                //printf("fd_chat.username = %s\n", fd_chat.username[i]);
                strcpy(fd_chat.send_username[i], row[1]);
                //printf("fd_chat.send_username = %s\n", fd_chat.send_username[i]);
                strcpy(fd_chat.message[i], row[2]);
                //printf("fd_chat.message = %s\n", fd_chat.message[i]);
                i++;
            }
        }
    }
    return fd_chat;
}


void  MYSQL_creat_gp(PACK pack)
{
    accept_mysql();
    char buff[200];
    sprintf(buff, "insert into groups(group_own, group_name) values ('%s','%s')", pack.username, pack.send_username);
    ret = mysql_query(&mysql, buff);
    sprintf(buff, "insert into group_member (group_name, group_user, flag) values ('%s', '%s', 1)", pack.send_username, pack.username);
    ret = mysql_query(&mysql, buff);
    printf("群%s创建成功\n", pack.send_username);
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
                //mysql_query(&mysql, "select *from group_member");
                sprintf(buff, "insert into group_member (group_name, group_user, flag) values ('%s', '%s', 0)", pack.send_username, pack.username);
                ret = mysql_query(&mysql, buff);
                //printf("%s\n", buff);
                flag = 1;
                break;
            }
            i++;
        }
    }
    //printf("i = %d\n", i);
    //printf("flag = %d", flag);
    return flag;
}

int MYSQL_quit_gp(PACK pack) 
{
    int flag = 0;
    char buff[200];
    accept_mysql();
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    if(result) {
        while((row = mysql_fetch_row(result))) {
            printf("群名 = %s   用户名 = %s\n", pack.send_username, pack.username);
            if((strcmp(row[0], pack.send_username) ==0 && strcmp(row[1], pack.username) == 0)) {
                flag = 1;
                sprintf(buff,"delete from group_member where (group_name = '%s' and group_user = '%s')", pack.send_username, pack.username);
                printf("%s\n", buff);
                ret = mysql_query(&mysql, buff);
                break;
            }
        }
    }
    printf("flag = %d\n", flag);
    return flag;
}

chat_GP MYSQL_chat_gp(PACK pack, chat_GP chat_gp)
{
    accept_mysql();
    char buff[200];
    sprintf(buff, "insert into gp_chatstore (group_name, group_username, mess) values ('%s', '%s', '%s')", pack.send_username, pack.username, pack.mess);
    ret = mysql_query(&mysql,buff);
    int i = 0;
    mysql_query(&mysql, "select *from group_member");
    result = mysql_store_result(&mysql);
    memset(&chat_gp, 0, sizeof(chat_GP));
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[0], pack.send_username) == 0) {
                printf("chat_gp.send_username[%d] = %s\n", i,  row[1]);
                strcpy(chat_gp.send_username[i], row[1]);
                i++;
            }
        }
    }
    strcpy(chat_gp.send_username[i++], "bye");
    printf("chat_gp.send_usernamep[%d] = %s\n",i , chat_gp.send_username);
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
    if(result) {
        while(row = mysql_fetch_row(result)) {
            if(strcmp(row[0], pack.send_username) == 0) {
                strcpy(gp_user_list.list[i], row[1]);
                i++;
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
    memset(&gp_chatstore, 0, sizeof(GP_chatstore));
    if(result) {
        while(row = mysql_fetch_row(result)) {
            //printf("pack.username = %s\n", pack.username);
            //printf("pack.send_username = %s\n", pack.send_username);
            if(strcmp(row[0], pack.send_username) == 0) {
                strcpy(gp_chatstore.username[i], row[1]);
                //printf("fd_chat.username = %s\n", fd_chat.username[i]);
                //strcpy(gp_chatstore.send_username[i], row[1]);
                //printf("fd_chat.send_username = %s\n", fd_chat.send_username[i]);
                strcpy(gp_chatstore.mess[i], row[2]);
                //printf("fd_chat.message = %s\n", fd_chat.message[i]);
                i++;
            }
        }
    }
    return gp_chatstore;
}
