#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "con_server.h"
#include "log.h"
#include "error.h"
#include "cmd.h"
#include "utils.h"
#include "game_mng.h"

int init_gm()
{
//   gs_info_t *ptr;
//    free_info_t *temp;
    int ret;

    log_msg(__FUNCTION__);

    // init cm
    gm.gs_num = 0;
    gm.gm_gs_blocks.next = NULL;
    if(gm_create_block() == NULL)
    {
        log_msg("gm_create_block() failed");
        return -1;
    }

    // init cm_free
    gm_free.info = NULL;
    gm_free.next = NULL;
    ret = gm_build_free(gm.gm_gs_blocks.next);
    if(ret != 0)
    {
        log_msg("gm_build_free() failed");
        return -1;
    }

    // init cm_online
    gm_online.cnt = 0;
    gm_online.free = 0;
    ret = gm_expand_online();
    if(ret != 0)
    {
        log_msg("gm_expand_online() failed");
        return -1;
    }

    gm_set_timer(GM_TIMER_SEC, GM_TIMER_USEC);
    return 0;
}

// add a cs to cm
int gm_add_gs(unsigned long s_addr, int port, int client_num)
{
    gs_info_t *tmp;
    gs_block_t *ptr;
    free_gs_info_t *fin;
    int ret;

    log_msg(__FUNCTION__);

    if(gm_free.next == NULL)
    {
        ptr = gm_create_block();
        if(ptr == NULL)
        {
            return -1;
        }
        ret = gm_build_free(ptr); // init free中也用这个函数
        if(ret != 0)
        {
            return -1;
        }

    }
    tmp = gm_free.next->info;
    fin = gm_free.next;     // fin is used to release the memory
    gm_free.next = gm_free.next->next;
    free(fin);
    fin = NULL;
    tmp->s_addr = s_addr;
    tmp->port = port;
    tmp->client_num = client_num;
    tmp->update_flag = 1;



    if(gm_online.free == 0)
    {
        ret = gm_expand_online();
        if(ret != 0)
        {
            return -1;
        }
    }
    gm_online.ptrs[gm_online.cnt] = tmp;
    gm_online.cnt ++;
    gm_online.free --;

    gm.gs_num ++;

    return 0;
}


// update information of a cs
int gm_update_gs(unsigned long s_addr, int port, int client_num)
{
    gs_info_t *ptr;

    log_msg(__FUNCTION__);

    ptr = gm_search_gs_by_addr(s_addr);
    if(ptr == NULL)
    {
        log_err("gm_update_gs()", ERR_NO_CS);
        return -1;
    }
    ptr->port = port;
    ptr->client_num = client_num;
    ptr->update_flag = 1;

    return 0;
}

// remove a cs from cm
int gm_remove_gs(unsigned long s_addr)
{
    gs_info_t *ptr;
//    free_info_t *fin;
    int ret;

    log_msg(__FUNCTION__);

    ptr = gm_search_gs_by_addr(s_addr);
    if(ptr == NULL)
    {
        log_err("gm_update_gs()", ERR_NO_CS);
        return -1;
    }

    ret = gm_remove_gs_by_infoaddr(ptr);
    if(ret != 0)
    {
        log_msg("gm_remove_gs_by_infoaddr() failed in gm_remove_gs()");
        return -1;
    }

//    gm.gs_num --;         // gm_remove_gs_by_infoaddr 中有减去

    return 0;
}

// remove a cs by its info addr in cm block
int gm_remove_gs_by_infoaddr(gs_info_t *info)
{
    gs_info_t **temp;
    free_gs_info_t *fin;

    log_msg(__FUNCTION__);

    memset(info, 0, sizeof(gs_info_t));

    temp = gm_online_search_by_ptr(info);
    if(temp == NULL)
    {
        log_msg("gm_online_search_by_ptr() in gm_remove_gs_by_infoaddr() return NULL");
        return -1;
    }
    memset(temp, 0, sizeof(gs_info_t *));

    gm.gs_num --;

    fin = (free_gs_info_t *)malloc(sizeof(free_gs_info_t));
    if(fin == NULL)
    {
        log_err("cm_remove_cs() malloc failed", ERR_MALLOC_FAILED);
        return -1;
    }

    fin->info = info;
    fin->next = gm_free.next;
    gm_free.next = fin;



    return 0;
}

// create a block of gs_info_t and add it to cm
gs_block_t *gm_create_block()
{
    gs_block_t *ptr, *tmp;
    gs_info_t *info;

    log_msg(__FUNCTION__);

    ptr = &gm.gm_gs_blocks;
    while(ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    tmp = (gs_block_t *)malloc(sizeof(gs_block_t));
    if(tmp == NULL)
    {
        log_err("gm_create_block()", ERR_MALLOC_FAILED);
        return NULL;
    }
    tmp->next = NULL;

    info = (gs_info_t *)malloc(GM_GS_BLOCK_SIZE * sizeof(gs_info_t));
    if(info == NULL)
    {
        log_err("gm_create_block()", ERR_MALLOC_FAILED);
        return NULL;
    }
    memset(info, 0, GM_GS_BLOCK_SIZE * sizeof(gs_info_t));

    tmp->block = info;
    ptr->next = tmp;

    return tmp;
}

// when the cm_free has no free items,
// build the cm_free from a gs_block_t
int gm_build_free(gs_block_t *blk)
{
    gs_info_t *ptr;
    free_gs_info_t *temp;
    int i;

    log_msg(__FUNCTION__);

    ptr = blk->block + GM_GS_BLOCK_SIZE - 1;
    for(i=0; i<GM_GS_BLOCK_SIZE; i++)
    {
        temp = (free_gs_info_t *)malloc(sizeof(free_gs_info_t *));
        if(temp == NULL)
        {
            log_err("gm_build_free()", ERR_MALLOC_FAILED);
            return -1;
        }
        temp->info = ptr;
        temp->next = gm_free.next;
        gm_free.next = temp;
        ptr = ptr - 1;
    }

    return 0;
}


// when the cm_online->ptr is full
// expand the ptr list
int gm_expand_online()
{
    gs_info_t **tmp;

    log_msg(__FUNCTION__);

    tmp = (gs_info_t **)realloc(gm_online.ptrs,
                                (gm_online.cnt + GM_GS_BLOCK_SIZE) * sizeof(gs_info_t *));
    if(tmp == NULL)
    {
        log_err("gm_expand_online realloc failed", ERR_REALLOC_FAILED);
        return -1;
    }

    free(gm_online.ptrs);
    gm_online.ptrs = tmp;
    gm_online.free = GM_GS_BLOCK_SIZE;

    return 0;
}


// search a cs in cm by addr
gs_info_t *gm_search_gs_by_addr(unsigned long s_addr)
{
    gs_info_t *info;
    gs_block_t *blk;
    int i;

    log_msg(__FUNCTION__);

    blk = gm.gm_gs_blocks.next;
    while(blk != NULL)
    {
        info = blk->block;
        for(i=0; i< GM_GS_BLOCK_SIZE; i++)
        {
            if(info->s_addr == s_addr)
            {
                return info;
            }
            info++;
        }
        blk = blk->next;
    }

    return NULL;
}

// search a cs pointer by its' value
gs_info_t **gm_online_search_by_ptr(gs_info_t *ptr)
{
    gs_info_t **info;
    int i;

    log_msg(__FUNCTION__);

    info = gm_online.ptrs;
    for(i=0; i<gm_online.cnt; i++)
    {
        if(*info == ptr)
        {
            return info;
        }
    }

    return NULL;
}


// set the timer
int gm_set_timer(long sec, long usec)
{

    log_msg(__FUNCTION__);

    struct itimerval timer;
    timer.it_interval.tv_sec = sec;
    timer.it_interval.tv_usec = usec;
    timer.it_value.tv_sec = sec;
    timer.it_value.tv_usec = usec;

    setitimer(ITIMER_REAL, &timer, NULL);
    signal(SIGALRM, gm_check_set_update);

    return 0;
}


// if the cs update flag is 1, set it to 0;
// if it is 0, remove the cs.
void gm_check_set_update(int signum)
{
    gs_block_t *blk;
    gs_info_t *info;
    int i;

    log_msg(__FUNCTION__);

    printf("gm.gs_num: %d\n", gm.gs_num);
    blk = gm.gm_gs_blocks.next;
    while(blk != NULL)
    {
        info = blk->block;
        for(i=0; i<GM_GS_BLOCK_SIZE; i++)
        {
            if(info->update_flag == 1)
            {
                info->update_flag = -1;
            }
            else if(info->update_flag == 0)
            {
                continue;
            }
            else
            {
                gm_remove_gs_by_infoaddr(info);
            }
        }
        blk = blk->next;
    }

}


int gm_build_server(int port, int listen_num)
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
            log_err("cm_build_server()", ERR_SELECT_FAILED);
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

