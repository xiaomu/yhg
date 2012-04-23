#ifndef _CLIENT_H
#define _CLIENT_H

struct
{
    unsigned long cm_s_addr;    // ip
    int cm_port;
    unsigned long cs_s_addr;
    int cs_port;
}client;


int client_query_cs(unsigned long s_addr, int port);
int client_play_game(unsigned long s_addr, int port);
#endif
