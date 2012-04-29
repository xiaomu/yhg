/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:58
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "cmd_handler.h"
#include "con_mng.h"
#include "cmd.h"
#include "log.h"
#include "game_mng.h"

// read cmd_id and handle it with the responding function

int (*cmd_handler[HANDLER_NUM])(int fd, unsigned long s_addr, int *err);

void init_cmd_handler()
{

    log_msg(__FUNCTION__);

    cmd_handler[CS_NOTICE_CM] = handle_cs_notice_cm;
    cmd_handler[CLIENT_QUERY_CS] = handle_client_query_cs;
    cmd_handler[CLIENT_PLAY_GAME] = handle_client_play_game;
}

int handle_cs_notice_cm(int fd, unsigned long s_addr, int *err)
{
    cs_info_t *info;
    int port, client_num;
    int ret;

    log_msg(__FUNCTION__);

    read(fd, &port, sizeof(int));
    read(fd, &client_num, sizeof(int));

    info = cm_search_cs_by_addr(s_addr);
    if(info != NULL)
    {
        ret = cm_update_cs(s_addr, port, client_num);
        if(ret != 0)
        {
            log_msg("cm_update_cs() failed");
            return -1;
        }
    }
    else
    {
        ret = cm_add_cs(s_addr, port, client_num);
        if(ret != 0 )
        {
            log_msg("cm_add_cs() failed");
            return -1;
        }
    }

    return 0;
}


int handle_client_query_cs(int fd, unsigned long s_addr, int *err)
{
    cs_info_t *info;
    unsigned long cs_s_addr;
    int cs_port;

    if(cm_online.cnt > 0)
    {
        if(cm_online.turn >= cm_online.cnt)
        {
            cm_online.turn = cm_online.cnt -1;
        }

        info = cm_online.ptrs[cm_online.turn];
        cs_s_addr = info->s_addr;
        cs_port = info->port;
    }
    else
    {
        cs_s_addr = 0;
        cs_port = 0;
    }

    write(fd, &cs_s_addr, sizeof(unsigned long));
    write(fd, &cs_port, sizeof(int));

    return 0;
}


int handle_client_play_game(int fd, unsigned long s_addr, int *err)
{
    // this function need change
    char *temp = "client game success";

    write(fd, temp,strlen(temp));

    return 0;
}


int handle_gs_notice_gm(int fd, unsigned long s_addr, int *err)
{
    gs_info_t *info;
    int port, client_num;
    int ret;

    log_msg(__FUNCTION__);

    read(fd, &port, sizeof(int));
    read(fd, &client_num, sizeof(int));

    info = gm_search_gs_by_addr(s_addr);
    if(info != NULL)
    {
        ret = gm_update_gs(s_addr, port, client_num);
        if(ret != 0)
        {
            log_msg("gm_update_gs() failed");
            return -1;
        }
    }
    else
    {
        ret = gm_add_gs(s_addr, port, client_num);
        if(ret != 0 )
        {
            log_msg("gm_add_gs() failed");
            return -1;
        }
    }

    return 0;
}
