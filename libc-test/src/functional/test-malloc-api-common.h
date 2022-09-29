#ifndef TEST_ITERATE_DISABLE_COMMON_H
#define TEST_ITERATE_DISABLE_COMMON_H

#include "test.h"

int check_and_report(const char *message, int (*check_func)(void))
{
    t_printf("%s...", message);
    int ret = check_func();
    if (ret == 0) {
        t_printf("Success\n");
    } else {
        t_error("Failed\n");
    }
    return ret;
}

#endif //TEST_ITERATE_DISABLE_COMMON_H