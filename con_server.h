/*****************************************************************************
 *  Copyright          :  2012 All Rights Reserved.
 *
 *  Date               :  2012-04-30 21:41:23
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
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
      int cs_listen_num;
}cs;


int init_cs();
int cs_set_notice_cm_timer(int sec, int usec);
void cs_notice_cm(int signum);
int cs_build_server(int port, int listen_num);

#endif
