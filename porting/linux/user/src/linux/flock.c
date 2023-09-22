#include <sys/file.h>
#include "syscall.h"

int __flock(int fd, int op)
{
	return syscall(SYS_flock, fd, op);
}
weak_alias(__flock, flock);
