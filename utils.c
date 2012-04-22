/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:58
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "defines.h"

void date_time(date_time_t *dt)
{
    time_t myt;
    struct tm *mytm;

    time(&myt);
    mytm = localtime(&myt);

    memset(dt, 0, 20);
    strftime((char *)dt, 20, "%Y-%m-%d %H:%M:%S", mytm);

}


void help(char *menu[])
{
    char **ptr;

    ptr = menu;
    while(*ptr != NULL)
    {
        printf("%s", *ptr);
        *ptr ++;
    }

}
