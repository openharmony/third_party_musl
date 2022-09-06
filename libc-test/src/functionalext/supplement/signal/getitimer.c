#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include "functionalext.h"

static int count = 0;
void expireTimer(int signo)
{
    struct itimerval val;
    count++;
    if (count > 1) {
        getitimer(signo, &val);
        val.it_value.tv_sec = 0;
        val.it_value.tv_usec = 0;
        setitimer(signo, &val, NULL);
        count = 0;
    }
}

void signalHandler(int signo)
{
    switch (signo) {
        case SIGALRM:
            printf("catch a SIGALRM signal\n");
            expireTimer(signo);
            break;
        case SIGVTALRM:
            printf("catch a SIGVTALRM signal\n");
            break;
        case SIGPROF:
            printf("catch a SIGPROF signal\n");
            break;
        default:
            printf("no signal\n");
            break;
    }
}

/**
 * @tc.name      : getitimer_0100
 * @tc.desc      : Verify the value of the interval timer (parameter is ITIMER_REAL)
 * @tc.level     : Level 0
 */
void getitimer_0100(void)
{
    signal(SIGALRM, signalHandler);
    struct itimerval old;
    int result = getitimer(ITIMER_REAL, &old);
    EXPECT_EQ("getitimer_0100", result, 0);
}

/**
 * @tc.name      : getitimer_0200
 * @tc.desc      : Verify the value of the interval timer (parameter is ITIMER_VIRTUAL)
 * @tc.level     : Level 0
 */
void getitimer_0200(void)
{
    signal(SIGALRM, signalHandler);
    struct itimerval old;
    int result = getitimer(ITIMER_VIRTUAL, &old);
    EXPECT_EQ("getitimer_0200", result, 0);
}

/**
 * @tc.name      : getitimer_0300
 * @tc.desc      : Verify the value of the interval timer (parameter is ITIMER_PROF)
 * @tc.level     : Level 0
 */
void getitimer_0300(void)
{
    signal(SIGALRM, signalHandler);
    struct itimerval old;
    int result = getitimer(ITIMER_PROF, &old);
    EXPECT_EQ("getitimer_0300", result, 0);
}

int main(void)
{
    getitimer_0100();
    getitimer_0200();
    getitimer_0300();

    return t_status;
}