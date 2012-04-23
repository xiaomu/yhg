
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
#include "game_server.h"



 #if 0
int main(int argc, char *argv[])
{
    // get opt
    int opt;

    char *menu[] =
    {
        "game_server -p gs_port -m gm_ip -n gm_port\n",
        NULL
    };

    if(argc < 4)
    {
        help(menu);
        return -1;
    }

    while((opt = getopt(argc, argv, "p:m:n:")) != -1)
    {
        switch(opt)
        {
            case 'p':
                gs.gs_port = atoi(optarg);
            case 'm':
                gs.gm_ip = optarg;
                break;
            case 'n':
                gs.gm_port = atoi(optarg);
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


    init_gs();
    gs_set_notice_gm_timer(GS_NOTICE_GM_INTERVAL_SEC, GS_NOTICE_GM_INTERVAL_USEC);

    while(1)
    {
        sleep(3);
    }
    return 0;
}
 #endif

int init_gs()
{
    gs.cs_num = 0;

    return 0;
}


int gs_set_notice_cm_timer(int sec, int usec)
{
    struct itimerval timer;
    timer.it_interval.tv_sec = sec;
    timer.it_interval.tv_usec = usec;
    timer.it_value.tv_sec = sec;
    timer.it_value.tv_usec = usec;

    setitimer(ITIMER_REAL, &timer, NULL);
    signal(SIGALRM, gs_notice_gm);

    return 0;
}

void gs_notice_cm(int signum)
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result, cmd_id;
    int ret;

    /*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(gs.gm_ip);
    address.sin_port = htons(gs.gm_port);
    len = sizeof(address);

    /*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1)
    {
        log_err("gs_notice_cm()", ERR_CONNECT_FAILED);
        return;
    }
    log_msg("connect succeed");

    /*  We can now read/write via sockfd.  */

    cmd_id = GS_NOTICE_CM;
    ret = write(sockfd, &cmd_id, sizeof(int));
    if(ret == -1)
    {
        perror("write failed");
    }
    ret = write(sockfd, &gs.gs_port, sizeof(int));
    if(ret == -1)
    {
        perror("write failed");
    }
    ret = write(sockfd, &gs.cs_num, sizeof(int));
    if(ret == -1)
    {
        perror("write failed");
    }
//    send_cmd(sockfd, gs_NOTICE_CM, sizeof(int), &(gs.client_num), NULL);
    close(sockfd);

}



int gs_build_server(int port, int listen_num)
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int result;
    fd_set readfds, testfds;


    log_msg(__FUNCTION__);

   /*  Create and name a socket for the server.  */

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    server_len = sizeof(server_address);

    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    /*  Create a connection queue and initialize readfds to handle input from server_sockfd.  */

    listen(server_sockfd, listen_num);

    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);

    /*  Now wait for clients and requests.
        Since we have passed a null pointer as the timeout parameter, no timeout will occur.
        The program will exit and report an error if select returns a value of less than 1.  */

    while(1)
    {
        int fd;
        int nread;

        testfds = readfds;

        printf("server waiting\n");
        result = select(FD_SETSIZE, &testfds, (fd_set *)0,
                        (fd_set *)0, (struct timeval *) 0);

        if(result < 1)
        {
            perror("select() failed");
            log_err("gs_build_server()", ERR_SELECT_FAILED);
            continue;
        }

        /*  Once we know we've got activity,
            we find which descriptor it's on by checking each in turn using FD_ISSET.  */

        for(fd = 0; fd < FD_SETSIZE; fd++)
        {
            if(FD_ISSET(fd,&testfds))
            {

                /*  If the activity is on server_sockfd, it must be a request for a new connection
                    and we add the associated client_sockfd to the descriptor set.  */

                if(fd == server_sockfd)
                {
                    client_len = sizeof(client_address);
                    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
                    FD_SET(client_sockfd, &readfds);

                }

                /*  If it isn't the server, it must be client activity.
                    If close is received, the client has gone away and we remove it from the descriptor set.
                    Otherwise, we 'serve' the client as in the previous examples.  */

                else
                {
                    ioctl(fd, FIONREAD, &nread);

                    if(nread != 0)
                    {
                        log_msg("do recv_cmd()");
                        recv_cmd(fd, client_address.sin_addr.s_addr);
                        log_msg("done recv_cmd()");
                    }

                    close(fd);
                    FD_CLR(fd, &readfds);
                    //printf("removing client on fd %d\n", fd);

                }
            }
        }
    }
}

