#include <unistd.h>
#include "pthread_impl.h"
#include "syscall.h"

static pid_t __get_cached_pid()
{
	return __pthread_self()->cached_pid;
}

pid_t getpid(void)
{
	pid_t cached_pid = __get_cached_pid();
	if (cached_pid != 0) {
		return cached_pid;
	}
	return __syscall(SYS_getpid);
}
