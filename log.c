/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:58
 *  Author             :  Dengzhaoqun
 *****************************************************************************/


#include <stdio.h>

#include "utils.h"
#include "defines.h"
#include "error.h"

static FILE *fp_msg;
static FILE *fp_err;

// open log files
int init_log(char *msg_path, char *err_path)
{
    if(msg_path == NULL)
    {
        fp_msg = fdopen(2, "a");
    }
    else
    {
        fp_msg = fopen(msg_path, "a");
    }
    if(fp_msg == NULL)
    {
        perror(msg_path);
        return -1;
    }

    if(err_path == NULL)
    {
        fp_err = fdopen(2, "a");
    }
    else
    {
        fp_err = fopen(err_path, "a");
    }
    if(fp_err == NULL)
    {
        perror(err_path);
        return -1;
    }

    return 0;

}

// write msg to log
void log_msg(const char *msg)
{
    date_time_t dt;

    date_time(&dt);
    fprintf(fp_msg, "%s", (char *)&dt);
    fprintf(fp_msg, "---%s",msg);
    fprintf(fp_msg, "\n");

}

// write msg and error number msg to log
void log_err(char *msg, int err_num)
{
    date_time_t dt;

    date_time(&dt);
    fprintf(fp_err, "%s", (char *)&dt);
    fprintf(fp_err, "---%s", msg);
    fprintf(fp_err, "---err num: %d", err_num);
    fprintf(fp_err, " %s", err_msg[err_num]);
    fprintf(fp_err, "\n");
}

