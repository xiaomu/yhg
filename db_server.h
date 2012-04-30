/*****************************************************************************
 *  Copyright          :  2012 All Rights Reserved.
 *
 *  Date               :  2012-04-30 21:47:01
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#ifndef _DB_SERVER_H
#define _DB_SERVER_H

struct
{
    int port;
    int listen_num;
}db;

int db_build_server(int port, int listen_num);

#endif
