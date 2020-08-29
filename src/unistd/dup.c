#include <unistd.h>
#include "syscall.h"
#include <unsupported_api.h>

int dup(int fd)
{
	unsupported_api(__FUNCTION__);
	return syscall(SYS_dup, fd);
}
