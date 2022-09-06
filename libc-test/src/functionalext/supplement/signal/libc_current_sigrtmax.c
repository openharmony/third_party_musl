#include <signal.h>
#include "functionalext.h"
#define MAXPATH 1000

/*
 * @tc.name      : libc_current_sigrtmax_0100
 * @tc.desc      : Verify that the number of available signals can be obtained
 * @tc.level     : Level 0
 */
void libc_current_sigrtmax_0100(void)
{
    int result = __libc_current_sigrtmax();
    EXPECT_EQ("libc_current_sigrtmax_0100", result, 64);
}

int main()
{
    libc_current_sigrtmax_0100();

    return t_status;
}