#include <stdio.h>
#include "lib_support_001.h"

static void MyFunction()
{
    printf("Function in libprimary called\n");
    AnotherFunction1();
}

int main()
{
    MyFunction();
    return 0;
}
