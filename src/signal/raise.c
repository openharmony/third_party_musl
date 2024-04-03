#include <signal.h>
#include <stdint.h>
#include "syscall.h"
#include "pthread_impl.h"
#include <unistd.h>

int raise(int sig)
{
	sigset_t set;
	__block_app_sigs(&set);
	int ret = syscall(SYS_tgkill, syscall(SYS_getpid), __pthread_self()->tid, sig);
	__restore_sigs(&set);
	return ret;
}
