/*****************************************************************************
 *  Copyright          :  2012 All Rights Reserved.
 *
 *  Date               :  2012-04-30 21:47:06
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
#include "db_server.h"

int main(int argc, char *argv[])
{
    int opt;

    char *menu[] =
    {
        "client -p port -n listen_num\n",
        NULL
    };

    if(argc < 5)
    {
        help(menu);
        return -1;
    }

    while((opt = getopt(argc, argv, "p:n:")) != -1)
    {
        switch(opt)
        {
        case 'p':
            db.port = atoi(optarg);
        case 'n':
            db.listen_num = atoi(optarg);
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
    
    db_build_server(db.port, db.listen_num);

    return 0;
}

