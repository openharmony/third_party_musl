#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include "syscall.h"
#include "pthread_impl.h"

hidden pid_t __vfork(void)
{
	/* vfork syscall cannot be made from C code */
#ifdef SYS_fork
	return syscall(SYS_fork);
#else
	return syscall(SYS_clone, SIGCHLD, 0);
#endif
}

pid_t vfork(void)
{
	pthread_t self = __pthread_self();
	pid_t parent_pid = self->pid;
	self->pid = 0;
	pid_t ret = __vfork();
	if (ret != 0) {
		self->pid = parent_pid;
	}
	return ret;
}
