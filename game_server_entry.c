/*****************************************************************************
 *  Copyright          :  2012 All Rights Reserved.
 *
 *  Date               :  2012-04-30 21:49:25
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

#include "con_server.h"
#include "log.h"
#include "error.h"
#include "cmd.h"
#include "utils.h"
#include "game_server.h"
#include "cmd_handler.h"

int main(int argc, char *argv[])
{
    // get opt
    int opt;

    char *menu[] =
    {
        "game_server -p gs_port -m gm_ip -n gm_port\n",
        NULL
    };

    if(argc < 4)
    {
        help(menu);
        return -1;
    }

    while((opt = getopt(argc, argv, "p:m:n:")) != -1)
    {
        switch(opt)
        {
            case 'p':
                gs.gs_port = atoi(optarg);
            case 'm':
                gs.gm_ip = optarg;
                break;
            case 'n':
                gs.gm_port = atoi(optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;

        }
    }


    init_log(NULL, NULL);

    log_msg(__FUNCTION__);

    init_err_msg();
    init_cmd_handler();


    init_gs();
    gs_set_notice_gm_timer(GS_NOTICE_GM_INTERVAL_SEC, GS_NOTICE_GM_INTERVAL_USEC);

    while(1)
    {
        sleep(3);
    }
    return 0;
}

