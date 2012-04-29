#ifndef _GAME_SERVER_H
#define _GAME_SERVER_H


#define GS_MAX_CS 1000

#define GS_NOTICE_GM_INTERVAL_SEC 15
#define GS_NOTICE_GM_INTERVAL_USEC 0

struct
{
      int cs_num;
      char *db_server_ip;
      int db_server_port;
      char *gm_ip;
      int gm_port;
      int gs_port;
}gs;


int init_gs();
int gs_set_notice_gm_timer(int sec, int usec);
void gs_notice_gm(int signum);
#endif
