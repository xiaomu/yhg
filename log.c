/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:58
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
// add time to log msg

log_msg(char *str); // log.c
log_err(char *str, int err_num); // log.c


#include <stdio.h>

#include "utils.h"

static FILE *fp_msg;
static FILE *fp_err;

// open log files
int init_log(char *msg_path, char *err_path)
{
      fp_msg = fopen(msg_path, "a");
      if(fp_msg == NULL)
      {
            perror(msg_path);
            return -1;
      }

      fp_err = fopen(err_path, "a");
      if(fp_err == NULL)
      {
            perror(err_path);
            return -1
      }

      return 0;

}

// write msg to log
void log_msg(char *msg)
{


}