#ifndef _CON_SERVER_H
#define _CON_SERVER_H

#define CS_MAX_CLIENT 1000

#define CS_NOTICE_CM_INTERVAL_SEC 3
#define CS_NOTICE_CM_INTERVAL_USEC 0


static char *cm_ip;
static int cm_port;
static char *gm_ip;
static int gm_port;


struct
{
      int client_num;
}cs;


int init_cs();
int cs_set_notice_cm_timer(int sec, int usec);
int cs_notice_cm(int signum);

#endif
