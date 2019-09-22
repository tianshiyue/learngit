/*************************************************************************
	> File Name: MYSQL.h
	> Author: 
	> Mail: 
	> Created Time: 2019年09月09日 星期一 19时42分34秒
 ************************************************************************/

#ifndef _MYSQL_H
#define _MYSQL_H

#include "define.h"

void Init_mysql();
int MYSQL_regist(PACK);
int MYSQL_login(PACK);
void MYSQL_exit(PACK);
int MYSQL_deal_addfd(PACK);
int MYSQL_addfd(PACK);
void MYSQL_delfd(PACK);
int MYSQL_deal_chat_fd(PACK);
void MYSQL_chat_fd(PACK);
FD_list MYSQL_fd_list(PACK,FD_list);
FD_chat MYSQL_fd_chatstore(PACK, FD_chat);
void  MYSQL_creat_gp(PACK);
int MYSQL_join_gp(PACK);
int MYSQL_quit_gp(PACK);
chat_GP MYSQL_chat_gp(PACK, chat_GP);
GP_list MYSQL_gp_list(PACK, GP_list);
GP_user_list MYSQL_gp_user_list(PACK, GP_user_list);
GP_chatstore MYSQL_gp_chatstore(PACK, GP_chatstore);


#endif
