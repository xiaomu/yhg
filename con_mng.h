/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:06:00
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#ifndef _CON_MNG_H
#define _CON_MNG_H

#define CM_CS_BLOCK_SIZE 3
#define CM_TIMER_SEC 30
#define CM_TIMER_USEC 0

#include <netinet/in.h>

typedef struct
{
//	struct sockaddr_in cs_addr;
    unsigned long int s_addr;   // ip地址中的四个字节组成一个32位的值 -- 通过ip唯一标识节点
    int port;
	int client_num;
	int update_flag;
}cs_info_t;

typedef struct CS_block
{
	cs_info_t *block;
	struct CS_block *next;
}cs_block_t;

struct
{
	cs_block_t cm_cs_blocks;
	int cs_num;
	int cm_port;
	int listen_num;
}cm;

struct
{
	int cnt;
	int free;
	int turn;
	cs_info_t **ptrs;
}cm_online;

typedef struct Free_info
{
	cs_info_t *info;
	struct Free_info *next;
}free_info_t;
free_info_t cm_free;



// functions
int init_cm();
cs_block_t *cm_create_block();
int cm_build_free(cs_block_t *blk);
int cm_expand_online();
int cm_set_timer(long sec, long usec);
int cm_remove_cs_by_infoaddr(cs_info_t *info);
cs_info_t *cm_search_cs_by_addr(unsigned long s_addr);
cs_info_t **cm_online_search_by_ptr(cs_info_t *ptr);
void cm_check_set_update(int signum);
int cm_build_server(int port, int listen_num);
int cm_add_cs(unsigned long s_addr, int port, int client_num);
int cm_update_cs(unsigned long s_addr, int port, int client_num);

#endif







