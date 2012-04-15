// cs list, add,del
// cs socket connect with clients
/*

	cm_cs -> cs_block;
	cs_block
	{
		*pre;
		*block;
		*next;
	}

	online -- array of ptrs;
	free   -- list of ptrs;
*/

int main()
{

	init_cm();

	return 0;
}

int init_cm()
{
	cs_info *ptr;
	int i;

	cm.cm_cs_blocks = (*cs_block)malloc(sizeof(cs_block));
	if(cm.cm_cs_blocks == NULL)
	{
		perror("malloc failed");
		return -1;
	}

	cm.cm_cs_blocks.next = NULL;
	cm.cm_cs_blocks.block = (*cs_info)malloc(CM_CS_BLOCK_SIZE * sizeof(cs_info));
	if(cm.cm_cs_blocks.block == NULL)
	{
		perror("malloc failed");
		return -1;
	}
	memset(cm.cm_cs_blocks.block, 0, CM_CS_BLOCK_SIZE * sizeof(cs_info));

	cm_online.cnt = 0;
	ptrs = (**cs_info)malloc(CM_CS_BLOCK_SIZE * sizeof(*cs_info));
	if(ptrs == NULL)
	{
		perror("malloc failed");
		return -1;
	}
	memset(ptrs, 0, CM_CS_BLOCK_SIZE * sizeof(*cs_info));

	cm_free.info = NULL;
	cm_free.next = NULL;	
	ptr = cm.cm_cs_blocks.block + CM_CS_BLOCK_SIZE - 1;
	for(i=0; i<CM_CS_BLOCK_SIZE; i++)
	{

		
		

	






