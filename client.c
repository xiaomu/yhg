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

// arg -- cm_ip cm_port
int main()
{
    int opt;
    int ret;

    char *menu[] =
    {
        "client -i ip -p port\n",
        NULL
    };

    if(argc < 5)
    {
        help(menu);
        return -1;
    }

    while((opt = getopt(argc, argv, "p:n:")) != -1)
    {
        switch(opt)
        {
        case 'i':
            client.cm_s_addr = inet_addr(optarg);
        case 'p':
            client.cm_port = atoi(optarg);
            break;
        case ':':
            printf("option needs a value\n");
            break;
        case '?':
            printf("unknown option: %c\n", optopt);
            break;

        }
    }

    init_log(NULL, NULL);

    log_msg(__FUNCTION__);

    init_err_msg();
    init_cmd_handler();

    ret = client_query_cs(client.cm_s_addr, client.cm_port);
    if(ret != 0)
    {
        log_msg("client_query_cs() failed");
        return -1;
    }

    ret = client_play_game(client.cs_s_addr, client.cs_port);
    if(ret != 0)
    {
        log_msg("client_play_game() failed");
        return -1;
    }

    log_msg("client --- finished");
    printf("client --- finished");

    return 0;
}


int client_query_cs(unsigned long s_addr, int port)
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result, cmd_id;
    int ret;

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
    int ret;
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
