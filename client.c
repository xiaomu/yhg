#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

#include "con_server.h"
#include "log.h"
#include "error.h"
#include "cmd.h"
#include "utils.h"
#include "client.h"
#include "cmd_handler.h"


int client_query_cs(unsigned long s_addr, int port)
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result, cmd_id;

    log_msg(__FUNCTION__);

    /*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = s_addr;
    address.sin_port = htons(port);
    len = sizeof(address);

    /*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1)
    {
        log_err("cs_notice_cm()", ERR_CONNECT_FAILED);
        return -1;
    }
    log_msg("connect succeed");

    /*  We can now read/write via sockfd.  */

    cmd_id = CLIENT_QUERY_CS;
    write(sockfd, &cmd_id, sizeof(int));

//  send_cmd(sockfd, CS_NOTICE_CM, sizeof(int), &(cs.client_num), NULL);

    read(sockfd, &client.cs_s_addr, sizeof(unsigned long));
    read(sockfd, &client.cs_port, sizeof(unsigned long));
    close(sockfd);

    return 0;
}


int client_play_game(unsigned long s_addr, int port)
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result, cmd_id;
    char msg[100];

    log_msg(__FUNCTION__);

    /*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = s_addr;
    address.sin_port = htons(port);
    len = sizeof(address);

    /*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1)
    {
        log_err("cs_notice_cm()", ERR_CONNECT_FAILED);
        return -1;
    }
    log_msg("connect succeed");

    /*  We can now read/write via sockfd.  */

    cmd_id = CLIENT_PLAY_GAME;
    write(sockfd, &cmd_id, sizeof(int));

//  send_cmd(sockfd, CS_NOTICE_CM, sizeof(int), &(cs.client_num), NULL);

    memset(msg, 0, 100);
    read(sockfd, msg, 100);
    printf("cs return msg: %s\n", msg);
    close(sockfd);

    return 0;
}
