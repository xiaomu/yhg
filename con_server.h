#ifndef _CON_SERVER_H
#define _CON_SERVER_H

#define CS_MAX_CLIENT 1000

#define CS_NOTICE_CM_INTERVAL_SEC 15
#define CS_NOTICE_CM_INTERVAL_USEC 0



struct
{
      int client_num;
      char *cm_ip;
      int cm_port;
      char *gm_ip;
      int gm_port;
      int cs_port;
}cs;


int init_cs();
int cs_set_notice_cm_timer(int sec, int usec);
void cs_notice_cm(int signum);

#endif
