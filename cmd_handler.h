/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:06:00
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#ifndef _CMD_HANDLER_H
#define _CMD_HANDLER_H

#define HANDLER_NUM 1024

int (*cmd_handler[HANDLER_NUM])(int cmd_len, void *cmd_content, int *err);

#endif
