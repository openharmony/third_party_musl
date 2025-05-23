#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unsupported_api.h>

pid_t tcgetpgrp(int fd)
{
	int pgrp;
	UNSUPPORTED_API_VOID(LITEOS_A);
	if (ioctl(fd, TIOCGPGRP, &pgrp) < 0)
		return -1;
	return pgrp;
}
