#include <signal.h>
#include <errno.h>
#include <sigchain.h>

extern void intercept_sigprocmask(int how, sigset_t *restrict set);

int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict old)
{
	sigset_t tmpset = *set;
	intercept_sigprocmask(how, &tmpset);
	const sigset_t *new_set_ptr = &tmpset;
	int r = pthread_sigmask(how, new_set_ptr, old);
	if (!r) return r;
	errno = r;
	return -1;
}
