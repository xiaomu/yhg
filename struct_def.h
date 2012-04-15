#ifndef _STRUCT_DEF
#define _STRUCT_DEF

#include <netinet/in.h>

typedef struct
{
	struct sockaddr_in cs_addr;
	int client_num;
}cs_info;


#endif
