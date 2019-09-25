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
FD_list MYSQL_online_fd_list(FD_list);
FD_chat MYSQL_fd_chatstore(PACK, FD_chat);
int MYSQL_creat_gp(PACK);
int MYSQL_join_gp(PACK);
QUIT_gp MYSQL_quit_gp(PACK, QUIT_gp);
chat_GP MYSQL_chat_gp(PACK, chat_GP);
int MYSQL_deal_chat_gp(PACK);
GP_list MYSQL_gp_list(PACK, GP_list);
GP_user_list MYSQL_gp_user_list(PACK, GP_user_list);
GP_chatstore MYSQL_gp_chatstore(PACK, GP_chatstore);
int MYSQL_untalk_gp(PACK);
void MYSQL_deal_untalk_gp(PACK);
int MYSQL_untalk1_gp(PACK);
void MYSQL_deal1_untalk_gp(PACK);
int MYSQL_deal_invite_user(PACK);
void MYSQL_invite_user_isok(PACK);
int MYSQL_deal_exit_gp(PACK);
void MYSQL_exit_gp_isok(PACK);


#endif
