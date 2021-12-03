#include <sched.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "test.h"

#define TEST(c) ((!c) ? 1 : (t_error(#c" failed: %s" ,strerror(errno)),0))

int main(int argc,char *argv[])
{
    struct sched_param param;
    int maxpri;

    maxpri = sched_get_priority_max(SCHED_FIFO);
    if(maxpri == -1) 
    {
        perror("get priority failed");
        return -1;
    }
    param.sched_priority = maxpri;
    TEST(sched_setscheduler(getpid(), SCHED_FIFO, &param));
    TEST(!(sched_getscheduler(getpid()) == SCHED_FIFO));
    
    return 0;
}

