/*****************************************************************************
 *  Copyright          :  2012 All Rights Reserved.
 *
 *  Date               :  2012-04-30 21:40:42
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "con_mng.h"
#include "error.h"
#include "log.h"
#include "cmd.h"
#include "utils.h"
#include "cmd_handler.h"

// add, remove cs 操作都只在 cm_timer 触发的时候执行
// 这样就不用对每一个 cs 设置 timer 了
// 使用 update_flag ， 可以随时调用 add, remove cs 了;

// 先不要考虑太多速度， 等后面出现瓶颈时再优化

// #if 0
int main(int argc, char *argv[])
{
    int opt;
    int ret;



    char *menu[] =
    {
        "con_mng -p port -n listen_num\n",
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
            cm.cm_port = atoi(optarg);
        case 'n':
            cm.listen_num = atoi(optarg);
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

    init_cm();

    ret = cm_build_server(cm.cm_port, cm.listen_num);
    if(ret != 0)
    {
        log_msg("cm_build_server() failed");
        return -1;
    }

    cm_set_timer(CM_TIMER_SEC, CM_TIMER_USEC);

    return 0;
}
// endif
