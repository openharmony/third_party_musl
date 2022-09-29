#include <malloc.h>
#include "test-malloc-api-common.h"

static int test_mallopt()
{
    return mallopt(0, 0) != 0;
}

int main()
{
    return check_and_report("Testing mallopt stub", test_mallopt);
}