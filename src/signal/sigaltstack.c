#include <signal.h>
#include <errno.h>
#include "syscall.h"
#include <unsupported_api.h>

int sigaltstack(const stack_t *restrict ss, stack_t *restrict old)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	if (ss) {
		if (!(ss->ss_flags & SS_DISABLE) && ss->ss_size < MINSIGSTKSZ) {
			errno = ENOMEM;
			return -1;
		}
		if (ss->ss_flags & SS_ONSTACK) {
			errno = EINVAL;
			return -1;
		}
	}
	return syscall(SYS_sigaltstack, ss, old);
}
