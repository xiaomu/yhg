#ifndef _CON_MNG_H
#define _CON_MNG_H

#define CM_CS_BLOCK_SIZE 20

typedef struct
{
	cs_info *block;
	cs_block *next;
}cs_block;

struct 
{
	cs_block *cm_cs_blocks;
}cm;

struct
{
	int cnt;
	cs_info **ptrs;
}cm_online;

typedef struct
{
	cs_info *info;
	free_info *next;
}free_info;

free_info cm_free;

#endif







