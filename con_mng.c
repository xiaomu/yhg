/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:58
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "con_mng.h"

int main()
{

	init_cm();

	return 0;
}

int init_cm()
{
	cs_info_t *ptr;
	free_info_t *temp;
	int i;

      // init cm
	cm.cm_cs_blocks = (*cs_block_t)malloc(sizeof(cs_block_t));
	if(cm.cm_cs_blocks == NULL)
	{
		perror("malloc failed");
		return -1;
	}
	cm.cm_cs_blocks.next = NULL;
	cm.cm_cs_blocks.block = (*cs_info_t)malloc(CM_CS_BLOCK_SIZE * sizeof(cs_info_t));
	if(cm.cm_cs_blocks.block == NULL)
	{
		perror("malloc failed");
		return -1;
	}
	memset(cm.cm_cs_blocks.block, 0, CM_CS_BLOCK_SIZE * sizeof(cs_info_t));

      // init cm_online
	cm_online.cnt = 0;
	cm_online.ptrs = (**cs_info_t)malloc(CM_CS_BLOCK_SIZE * sizeof(*cs_info_t));
	if(ptrs == NULL)
	{
		perror("malloc failed");
		return -1;
	}
	memset(cm_online.ptrs, 0, CM_CS_BLOCK_SIZE * sizeof(*cs_info_t));
	cm_online.free = CM_CS_BLOCK_SIZE;

      // init cm_free
	cm_free.info = NULL;
	cm_free.next = NULL;
	ptr = cm.cm_cs_blocks.block + CM_CS_BLOCK_SIZE - 1;
	for(i=0; i<CM_CS_BLOCK_SIZE; i++)
	{
	      temp = (free_info_t *)malloc(sizeof(free_info_t *));
	      if(temp == NULL)
	      {
	            perror("malloc failed");
	            return -1;
	      }
	      temp->info = ptr;
	      temp->next = cm_free.next;
	      cm_free.next = temp;
	}

	return 0;
}

// add, remove 操作都只在 cm_timer 触发的时候执行
// 这样就不用对每一个 cs 设置 timer 了
cm_add_cs();
cm_remove_cs();
cm_update_cs();







