/*****************************************************************************
 *  Copyright          :  2012 All Rights Reserved.
 *
 *  Date               :  2012-04-30 21:39:41
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
#include "client.h"
#include "cmd_handler.h"


// arg -- cm_ip cm_port
int main(int argc, char *argv[])
{
    int opt;
    int ret;

    char *menu[] =
    {
        "client -i ip -p port\n",
        NULL
    };

    if(argc < 5)
    {
        help(menu);
        return -1;
    }

    while((opt = getopt(argc, argv, "i:p:")) != -1)
    {
        switch(opt)
        {
        case 'i':
            client.cm_s_addr = inet_addr(optarg);
        case 'p':
            client.cm_port = atoi(optarg);
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

    ret = client_query_cs(client.cm_s_addr, client.cm_port);
    if(ret != 0)
    {
        log_msg("client_query_cs() failed");
        return -1;
    }

    ret = client_play_game(client.cs_s_addr, client.cs_port);
    if(ret != 0)
    {
        log_msg("client_play_game() failed");
        return -1;
    }

    log_msg("client --- finished");
    printf("client --- finished");

    return 0;
}

