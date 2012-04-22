#include <stdio.h>
#include <unistd.h>

#include "cmd_handler.h"
#include "con_mng.h"
#include "cmd.h"
#include "log.h"

// read cmd_id and handle it with the responding function

int (*cmd_handler[HANDLER_NUM])(int fd, unsigned long s_addr, int *err);

void init_cmd_handler()
{
    cmd_handler[CS_NOTICE_CM] = handle_cs_notice_cm;
}

int handle_cs_notice_cm(int fd, unsigned long s_addr, int *err)
{
    cs_info_t *info;
    int port, client_num;
    int ret;

    read(fd, &port, sizeof(int));
    read(fd, &client_num, sizeof(int));

    info = cm_serch_cs_by_addr(s_addr);
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
