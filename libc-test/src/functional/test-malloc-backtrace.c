#include <malloc.h>
#include "test-malloc-api-common.h"

static int test_backtrace()
{
    return malloc_backtrace(NULL, NULL, 0) != 0;
}

int main()
{
    return check_and_report("Testing malloc_backtrace stub", test_backtrace);
}