/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:06:00
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#ifndef _CMD_HANDLER_H
#define _CMD_HANDLER_H

#define HANDLER_NUM 1024

int (*cmd_handler[HANDLER_NUM])(int fd, unsigned long s_addr, int *err);

int handle_cs_notice_cm(int fd, unsigned long s_addr, int *err);
int handle_client_query_cs(int fd, unsigned long s_addr, int *err);
int handle_client_play_game(int fd, unsigned long s_addr, int *err);
int handle_gs_notice_gm(int fd, unsigned long s_addr, int *err);

void init_cmd_handler();

#endif
