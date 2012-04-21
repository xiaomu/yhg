/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:58
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#include "con_mng.h"
#include "error.h"

// add, remove cs 操作都只在 cm_timer 触发的时候执行
// 这样就不用对每一个 cs 设置 timer 了
// 使用 update_flag ， 可以随时调用 add, remove cs 了;

// 先不要考虑太多速度， 等后面出现瓶颈时再优化


int main()
{

	init_cm();

	return 0;
}

int init_cm()
{
	cs_info_t *ptr;
	free_info_t *temp;
	int i, ret;

      // init cm
      cm.cs_num = 0;
	cm.cm_cs_blocks.next = NULL;
	if(cm_create_block() == NULL)
	{
	      log_msg("cm_create_block() failed");
	      return -1;
	}

      // init cm_free
	cm_free.info = NULL;
	cm_free.next = NULL;
	ret = cm_build_free(cm.cm_cs_blocks.block);
	if(ret != 0)
	{
	      log_msg("cm_build_free() failed");
	      return -1;
	}

	// init cm_online
	cm_online.cnt = 0;
	cm_online.free = 0;
	ret = cm_expand_online();
	if(ret != 0)
	{
	      log_msg("cm_expand_online() failed");
	      return -1;
	}

	cm_set_timer(CM_TIMER_SEC, CM_TIMER_USEC);
	return 0;
}

// add a cs to cm
int cm_add_cs(struct sockaddr_in *cs_addr, int client_num)
{
      cs_info_t *tmp;
      cs_block_t *ptr;
      free_info_t *fin;
      int ret;

      if(cm_free.next == NULL)
      {
            ptr = cm_create_block();
            if(ptr == NULL)
            {
                  return -1;
            }
            ret = cm_build_free(ptr) // init free中也用这个函数
            if(ret != 0)
            {
                  return -1;
            }

      }
      tmp = cm_free.next.info;
      fin = cm_free.next；     // fin is used to release the memory
      cm_free.next = cm_free.next.next;
      free(fin);
      fin = NULL;
      memcpy(tmp->cs_addr, cs_addr, sizeof(struct sockaddr_in));
      tmp->client_num = client_num;
      tmp->update_flag = 1;



      if(cm_online.free == 0)
      {
            ret = cm_expand_online();
            if(ret != 0)
            {
                  return -1;
            }
      }
      cm_online.ptrs[cnt] = tmp;
      cm_online.cnt ++;
      cm_online.free --;

      cm.cs_num ++;

      return 0;
}


// update information of a cs
int cm_update_cs(struct sockaddr_in *cs_addr, int client_num)
{
      cs_info_t *ptr;

      ptr = cm_serch_cs_by_addr(cs_addr);
      if(ptr == NULL)
      {
            log_err("cm_update_cs()", ERR_NO_CS);
            return -1;
      }
      ptr->client_num = client_num;
      ptr->update_flag = 1;

      return 0;
}

// remove a cs from cm
int cm_remove_cs(struct sockaddr_in *cs_addr)
{
      cs_info_t *ptr **temp;
      free_info_t *fin;
      int ret;

      ptr = cm_serch_cs_by_addr(cs_addr);
      if(ptr == NULL)
      {
            log_err("cm_update_cs()", ERR_NO_CS);
            return -1;
      }

      ret = cm_remove_cs_by_infoaddr(ptr);
      if(ret != 0)
      {
            log_msg("cm_remove_cs_by_infoaddr() failed in cm_remove_cs()");
            return -1;
      }

      cm.cs_num --;

      return 0;
}

// remove a cs by its info addr in cm block
int cm_remove_cs_by_infoaddr(cs_info_t *info)
{
      cs_info_t **temp;
      free_info_t *fin;

      memset(info, 0, sizeof(cs_info_t));

      temp = cm_online_search_by_ptr(info);
      memset(temp, 0, sizeof(cs_info_t *));

      fin = (free_info_t *)malloc(sizeof(free_info_t));
      if(fin == NULL)
      {
            log_err("cm_remove_cs() malloc failed", ERR_MALLOC_FAILED);
            return -1;
      }

      fin->info = ptr;
      fin->next = cm_free.next;
      cm_free.next = fin;

      return 0;
}

// create a block of cs_info_t and add it to cm
cs_block_t *cm_create_block()
{
      cs_block_t *ptr, *tmp;
      cs_info_t *info;

      ptr = cm.cm_cs_blocks;
      while(ptr->next != NULL)
      {
            ptr = ptr->next;
      }

      tmp = (cs_block_t *)malloc(sizeof(cs_block_t));
      if(tmp == NULL)
      {
            log_err("cm_create_block()", ERR_MALLOC_FAILED);
            return NULL;
      }
      tmp->next = NULL;

      info = (cs_info_t *)malloc(CM_CS_BLOCK_SIZE * sizeof(cs_info_t));
      if(info == NULL)
      {
            log_err("cm_create_block()", ERR_MALLOC_FAILED);
            return NULL;
      }
      memset(info, 0, CM_CS_BLOCK_SIZE * sizeof(cs_info_t));

      tmp->block = info;
      ptr->next = tmp;

      return tmp;
}

// when the cm_free has no free items,
// build the cm_free from a cs_block_t
int cm_build_free(cs_block_t *blk)
{
      cs_info_t *ptr;
      free_info_t *temp;

      ptr = blk->block + CM_CS_BLOCK_SIZE - 1;
      for(i=0; i<CM_CS_BLOCK_SIZE; i++)
      {
	      temp = (free_info_t *)malloc(sizeof(free_info_t *));
	      if(temp == NULL)
	      {
	            log_err("cm_create_block()", ERR_MALLOC_FAILED);
	            return -1;
	      }
	      temp->info = ptr;
	      temp->next = cm_free.next;
	      cm_free.next = temp;
	      ptr = ptr - 1;
	}

}


// when the cm_online->ptr is full
// expand the ptr list
int cm_expand_online()
{
      cs_info_t **tmp;
      tmp = (cs_info_t **)realloc(cm_online.ptrs,
                                  (cm_online.cnt + CM_CS_BLOCK_SIZE) * sizeof(cs_info_t *));
      if(tmp == NULL)
      {
            log_err("cm_expand_online realloc failed", ERR_REALLOC_FAILED);
            return -1;
      }

      free(cm_online.ptrs);
      cm_online.ptrs = tmp;
      cm_online.free = CM_CS_BLOCK_SIZE;

      return 0;
}


// search a cs in cm by addr
cs_info_t *cm_serch_cs_by_addr(struct sockaddr_in cs_addr)
{
      cs_info_t *info;
      cs_block_t *blk;
      int i, ret;

      blk = cm.cm_cs_blocks.next;
      while(blk != NULL)
      {
            info = blk->block;
            for(i=0; i< CM_CS_BLOCK_SIZE; i++)
            {
                  ret = memcmp(info->cs_addr, cs_addr, sizeof(struct sockaddr_in));
                  if(ret == 0)
                  {
                        return info;
                  }
            }
      }

      return NULL;
}

// search a cs pointer by its' value
cs_info_t **cm_online_search_by_ptr(cs_info_t *ptr)
{
      cs_info_t **info;
      int i;

      info = cm_online.ptrs;
      for(i=0; i<cm_online.cnt; i++)
      {
            if(*info == ptr)
            {
                  return info;
            }
      }

      return NULL;
}


// set the timer
int cm_set_timer(long sec, long usec)
{
      struct itimerval timer;
      timer.it_interval.tv_sec = sec;
      timer.it_interval.tv_usec = usec;
      timer.it_value.tv_sec = sec;
      timer.it_value.tv_usec = usec;

      setitimer(ITIMER_REAL, &timer, NULL);
      signal(SIGALRM, cm_check_set_update);

      return 0;
}


// if the cs update flag is 1, set it to 0;
// if it is 0, remove the cs.
int cm_check_set_update(int signum)
{
      cs_block_t *blk;
      cs_info_t *info;
      int i;

      blk = cm.cm_cs_blocks.next;
      while(blk != NULL)
      {
            info = blk->block;
            for(i=0; i<CM_CS_BLOCK_SIZE; i++)
            {
                  if(info->update_flag == 1)
                  {
                        info->update_flag = 0;
                  }
                  else
                  {
                        cm_remove_cs_by_infoaddr(info);
                  }
            }
      }

      return 0;
}


int cm_build_server(int port, int listen_num)
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int result;
    fd_set readfds, testfds;

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
        char ch;
        int fd;
        int nread;

        testfds = readfds;

        printf("server waiting\n");
        result = select(FD_SETSIZE, &testfds, (fd_set *)0,
                        (fd_set *)0, (struct timeval *) 0);

        if(result < 1)
        {
            perror("select() failed");
            log_err("select() failed", ERR_SELECT_FAILED);
            exit(1);
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
                    client_sockfd = accept(server_sockfd,
                                           (struct sockaddr *)&client_address, &client_len);
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
                        recv_cmd(fd);
                    }

                    close(fd);
                    FD_CLR(fd, &readfds);
                    //printf("removing client on fd %d\n", fd);

                }
            }
        }
    }
}


