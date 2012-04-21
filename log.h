/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:06:00
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#ifndef _LOG_H
#define _LOG_H

int init_log(char *msg_path, char *err_path);
void log_msg(char *msg);
void log_err(char *msg, int err_num);

#endif
