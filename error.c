/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:58
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
#include "error.h"

void init_err_msg()
{
    err_msg[ERR_MALLOC_FAILED] = "malloc failed";
    err_msg[ERR_REALLOC_FAILED] = "realloc failed";
    err_msg[ERR_NO_CS] = "NO CS";
    err_msg[ERR_SELECT_FAILED] = "select failed";
    err_msg[ERR_CONNECT_FAILED] = "connect failed";

}

