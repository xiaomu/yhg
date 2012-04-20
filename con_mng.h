/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:06:00
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#ifndef _CON_MNG_H
#define _CON_MNG_H

#define CM_CS_BLOCK_SIZE 20

#include <netinet/in.h>
typedef struct
{
	struct sockaddr_in cs_addr;
	int client_num;
}cs_info_t;

typedef struct
{
	cs_info_t *block;
	cs_block_t *next;
}cs_block_t;

struct
{
	cs_block_t *cm_cs_blocks;
}cm;

struct
{
	int cnt;
	int free;
	cs_info_t **ptrs;
}cm_online;

typedef struct
{
	cs_info_t *info;
	free_info_t *next;
}free_info_t;
free_info_t cm_free;

#endif







