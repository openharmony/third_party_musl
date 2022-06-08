#include <signal.h>
#include "syscall.h"

void (*signal(int sig, void (*func)(int)))(int)
{
	struct sigaction sa_old, sa = { .sa_handler = func, .sa_flags = SA_RESTART };
	if (__sigaction(sig, &sa, &sa_old) < 0)
		return SIG_ERR;
	return sa_old.sa_handler;
}


int tgkill(int __tgid, int __tid, int __signal){
    return __syscall(SYS_tgkill, __tgid, __tid, __signal);
}

weak_alias(signal, bsd_signal);
weak_alias(signal, __sysv_signal);
