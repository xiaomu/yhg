/*****************************************************************************
 *  Copyright          :  2012 All Rights Reserved.
 *
 *  Date               :  2012-04-30 21:41:29
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
#include "cmd_handler.h"


int main(int argc, char *argv[])
{
    // get opt
    int opt;

    char *menu[] =
    {
        "con_server -p cs_port -c cs_listen_num -m cm_ip -n cm_port\n",
        NULL
    };

    if(argc < 9)
    {
        help(menu);
        return -1;
    }

    while((opt = getopt(argc, argv, "p:c:m:n:")) != -1)
    {
        switch(opt)
        {
            case 'p':
                cs.cs_port = atoi(optarg);
            case 'c':
                cs.cs_listen_num = atoi(optarg);
            case 'm':
                cs.cm_ip = optarg;
                break;
            case 'n':
                cs.cm_port = atoi(optarg);
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


    init_cs();
    cs_set_notice_cm_timer(CS_NOTICE_CM_INTERVAL_SEC, CS_NOTICE_CM_INTERVAL_USEC);
    
    cs_build_server(cs.cs_port, cs.cs_listen_num);

    return 0;
}

