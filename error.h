/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:06:00
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#ifndef _ERROR_H
#define _ERROR_H

#define ERR_NUM 1024
char *err_msg[ERR_NUM];

#define ERR_MALLOC_FAILED 1
#define ERR_REALLOC_FAILED 2

#define ERR_NO_CS 101

#define ERR_SELECT_FAILED 201
#define ERR_CONNECT_FAILED 202

#endif
