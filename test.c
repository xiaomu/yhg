/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:58
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#include <stdio.h>

struct st
{
	int i;
};

struct st2
{
	struct st to;
};

int main()
{
	struct st2 to2;

	to2.to.i = 1;
	printf("%d\n", to2.to.i);

	return 0;

}
	

