#include <sched.h>
#include <errno.h>
#include <string.h>
#include "syscall.h"

int sched_getscheduler(pid_t pid)
{
	int r = __syscall(SYS_sched_getscheduler, pid);
	return  __syscall_ret(r);
}