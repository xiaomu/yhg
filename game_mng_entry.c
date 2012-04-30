/*****************************************************************************
 *  Copyright          :  2012 All Rights Reserved.
 *
 *  Date               :  2012-04-30 21:48:45
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
#include "game_mng.h"
#include "cmd_handler.h"


// #if 0
int main(int argc, char *argv[])
{
    int opt;
    int ret;



    char *menu[] =
    {
        "game_mng -p port -n listen_num\n",
        NULL
    };

    if(argc < 4)
    {
        help(menu);
        return -1;
    }

    while((opt = getopt(argc, argv, "p:n:")) != -1)
    {
        switch(opt)
        {
        case 'p':
            gm.gm_port = atoi(optarg);
        case 'n':
            gm.listen_num = atoi(optarg);
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

    init_gm();

    ret = gm_build_server(gm.gm_port, gm.listen_num);
    if(ret != 0)
    {
        log_msg("gm_build_server() failed");
        return -1;
    }


    return 0;
}
// endif
