/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:57
 *  Author             :  Dengzhaoqun
 *****************************************************************************/

#include "cmd.h"
#include "cmd_handler.h"


int recv_cmd(int fd)
{
      int cmd_id, len;
      char buf[CMD_LEN];
      int err;
      char tmp[20];

      read(fd, &cmd_id, sizeof(int));
      read(fd, &len, sizeof(int));
      memset(buf, 0, CMD_LEN);
      read(fd, buf, len);

      if(cmd_handler[cmd_id] != NULL)
      {
            err = 0;
            (cmd_handler[cmd_id])(len, (void *)buf, &err);
            if(err)
            {
                  sprintf(tmp, "(cmd_handler[%d]) failed", cmd_id);
                  log_err(tmp, err);
                  return -1;
            }
      }

      return 0;
}

int send_cmd(int fd, int cmd_id, int len, void *content, int *err)
{
      write(fd, cmd_id, sizeof(int));
      write(fd, len, sizeof(int));
      write(fd, content, len);

      return 0;
}
