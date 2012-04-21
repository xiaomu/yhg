/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:06:00
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#ifndef _CON_MNG_H
#define _CON_MNG_H

#define CM_CS_BLOCK_SIZE 20
#define CM_TIMER_SEC 10
#define CM_TIMER_USEC 0

#include <netinet/in.h>
typedef struct
{
	struct sockaddr_in cs_addr;
	int client_num;
	int update_flag;
}cs_info_t;

typedef struct
{
	cs_info_t *block;
	cs_block_t *next;
}cs_block_t;

struct
{
	cs_block_t cm_cs_blocks;
	int cs_num;
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



// functions
cs_block_t *cm_create_block();
int cm_build_free(cs_block_t *blk);
int cm_expand_online();
int cm_set_timer(long sec, long usec);
int cm_remove_cs_by_infoaddr(cs_info_t *info);
#endif







