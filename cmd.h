/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:05:59
 *  Author             :  Dengzhaoqun
 *****************************************************************************/


#ifndef _CMD_H
#define _CMD_H

#define CMD_LEN 128

#define CM_ADD_CS 1001
#define CM_REMOVE_CS 1002
#define CM_UPDATE_CS 1003
#define CS_NOTICE_CM 1004     //先只用这个， 来勉强实现上面三条命令的功能

int recv_cmd(int fd);
int send_cmd(int fd, int cmd_id, int len, void *content, int *err);

#endif
