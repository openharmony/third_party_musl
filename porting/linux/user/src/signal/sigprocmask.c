#include <signal.h>
#include <errno.h>
#include <sigchain.h>
#include <stddef.h>
#include <hilog_adapter.h>
#include <string.h>
#include "sys_param.h"

extern void intercept_sigprocmask(int how, sigset_t *restrict set);
static const char *param_name = "musl.sigchain.procmask";

bool get_sigchain_mask_enable()
{
    static CachedHandle musl_log_Handle = NULL;
    if (musl_log_Handle == NULL) {
        musl_log_Handle = CachedParameterCreate(param_name, "false");
    }
    char *param_value = CachedParameterGet(musl_log_Handle);

    if (param_value != NULL) {
        if (strcmp(param_value, "true") == 0) {
            return true;
        }
    }
    return false;
}

int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict old)
{
	int r = 0;
	if (set && get_sigchain_mask_enable()) {
		sigset_t tmpset = *set;
		intercept_sigprocmask(how, &tmpset);
		const sigset_t *new_set_ptr = &tmpset;
		r = pthread_sigmask(how, new_set_ptr, old);
	} else {
		r = pthread_sigmask(how, set, old);
	}
	if (!r) return r;
	errno = r;
	return -1;
}
