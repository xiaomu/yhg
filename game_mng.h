/*****************************************************************************
 *  Copyright          :  2012 All Rights Reserved.
 *
 *  Date               :  2012-04-30 21:48:42
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#ifndef _GAME_MNG_H
#define _GAME_MNG_H

#define GM_GS_BLOCK_SIZE 3
#define GM_TIMER_SEC 30
#define GM_TIMER_USEC 0

#include <netinet/in.h>

typedef struct
{
//	struct sockaddr_in cs_addr;
    unsigned long int s_addr;   // ip地址中的四个字节组成一个32位的值 -- 通过ip唯一标识节点
    int port;
	int client_num;
	int update_flag;
}gs_info_t;

typedef struct GS_block
{
	gs_info_t *block;
	struct GS_block *next;
}gs_block_t;

struct
{
	gs_block_t gm_gs_blocks;
	int gs_num;
	int gm_port;
	int listen_num;
}gm;

struct
{
	int cnt;
	int free;
	gs_info_t **ptrs;
}gm_online;

typedef struct GM_free_info
{
	gs_info_t *info;
	struct GM_free_info *next;
}free_gs_info_t;
free_gs_info_t gm_free;



// functions
int init_gm();
gs_block_t *gm_create_block();
int gm_build_free(gs_block_t *blk);
int gm_expand_online();
int gm_set_timer(long sec, long usec);
int gm_remove_gs_by_infoaddr(gs_info_t *info);
gs_info_t *gm_search_gs_by_addr(unsigned long s_addr);
gs_info_t **gm_online_search_by_ptr(gs_info_t *ptr);
void gm_check_set_update(int signum);
int gm_build_server(int port, int listen_num);
int gm_add_gs(unsigned long s_addr, int port, int client_num);
int gm_update_gs(unsigned long s_addr, int port, int client_num);


#endif
