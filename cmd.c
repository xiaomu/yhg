/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:57
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#include <stdio.h>
#include <unistd.h>

#include "cmd.h"
#include "cmd_handler.h"
#include "log.h"


int recv_cmd(int fd)
{
      int cmd_id;
      int err;
      char tmp[20];

      read(fd, &cmd_id, sizeof(int));


      if(cmd_handler[cmd_id] != NULL)
      {
            err = 0;
            (cmd_handler[cmd_id])(fd, &err);
            if(err)
            {
                  sprintf(tmp, "(cmd_handler[%d]) failed", cmd_id);
                  log_err(tmp, err);
                  return -1;
            }
      }

      return 0;
}


