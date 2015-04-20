#include "dbg.h"
#include <stdlib.h>
#include <stdio.h>

int test_hello()
{
    log_info("******************************************");
    log_info("hello");
    log_info("******************************************");

    return 0;
error:
    return 1;
}
