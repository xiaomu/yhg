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

 #if 0
int main(int argc, char *argv[])
{
    // get opt
    int opt;

    char *menu[] =
    {
        "con_server -p cs_port -m cm_ip -n cm_port\n",
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
                cs.cs_port = atoi(optarg);
            case 'm':
                cs.cm_ip = optarg;
                break;
            case 'n':
                cs.cm_port = atoi(optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;

        }
    }

    init_err_msg();
    init_log(NULL, NULL);
    init_cs();
    cs_set_notice_cm_timer(CS_NOTICE_CM_INTERVAL_SEC, CS_NOTICE_CM_INTERVAL_USEC);

    while(1)
    {
        sleep(3);
    }
    return 0;
}
 #endif

int init_cs()
{
    cs.client_num = 0;

    return 0;
}


int cs_set_notice_cm_timer(int sec, int usec)
{
    struct itimerval timer;
    timer.it_interval.tv_sec = sec;
    timer.it_interval.tv_usec = usec;
    timer.it_value.tv_sec = sec;
    timer.it_value.tv_usec = usec;

    setitimer(ITIMER_REAL, &timer, NULL);
    signal(SIGALRM, cs_notice_cm);

    return 0;
}

void cs_notice_cm(int signum)
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
    address.sin_addr.s_addr = inet_addr(cs.cm_ip);
    address.sin_port = htons(cs.cm_port);
    len = sizeof(address);

    /*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1)
    {
        log_err("cs_notice_cm()", ERR_CONNECT_FAILED);
        return;
    }
    log_msg("connect succeed");

    /*  We can now read/write via sockfd.  */

    cmd_id = CS_NOTICE_CM;
    ret = write(sockfd, &cmd_id, sizeof(int));
    if(ret == -1)
    {
        perror("write failed");
    }
    ret = write(sockfd, &cs.cs_port, sizeof(int));
    if(ret == -1)
    {
        perror("write failed");
    }
    ret = write(sockfd, &cs.client_num, sizeof(int));
    if(ret == -1)
    {
        perror("write failed");
    }
//    send_cmd(sockfd, CS_NOTICE_CM, sizeof(int), &(cs.client_num), NULL);
    close(sockfd);

}
