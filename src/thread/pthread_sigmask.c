#include <signal.h>
#include <errno.h>
#ifndef __LITEOS__
#include <sigchain.h>
#include <stddef.h>
#include <hilog_adapter.h>
#include <string.h>
#endif
#include "syscall.h"

#ifndef __LITEOS__
#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
#endif

extern void intercept_pthread_sigmask(int how, sigset_t *restrict set);
static const char *param_name = "musl.sigchain.procmask";

/**
  * @brief Get whether sigchain mask is enabled
  * @retval True if the sigchain mask is enable, or false.
  */
bool get_sigchain_mask_enable()
{
#ifdef OHOS_ENABLE_PARAMETER
	static CachedHandle sigchain_procmask_handle = NULL;
	if (sigchain_procmask_handle == NULL) {
		sigchain_procmask_handle = CachedParameterCreate(param_name, "false");
	}
	char *param_value = CachedParameterGet(sigchain_procmask_handle);
	if (param_value != NULL) {
		if (strcmp(param_value, "true") == 0) {
			return true;
		}
	}
#endif
	return false;
}
#endif

int pthread_sigmask(int how, const sigset_t *restrict set, sigset_t *restrict old)
{
	int ret;
	if (set && (unsigned)how - SIG_BLOCK > 2U) return EINVAL;
#ifndef __LITEOS__
	/* sigchain intercepts pthread_sigmask */
	if (set && get_sigchain_mask_enable()) {
		sigset_t tmpset = *set;
		intercept_pthread_sigmask(how, &tmpset);
		const sigset_t *new_set_ptr = &tmpset;
		ret = -__syscall(SYS_rt_sigprocmask, how, new_set_ptr, old, _NSIG / 8);
	} else {
		ret = -__syscall(SYS_rt_sigprocmask, how, set, old, _NSIG / 8);
	}
#else
	ret = -__syscall(SYS_rt_sigprocmask, how, set, old, _NSIG/8);
#endif
	if (!ret && old) {
		if (sizeof old->__bits[0] == 8) {
			old->__bits[0] &= ~0x380000000ULL;
		} else {
			old->__bits[0] &= ~0x80000000UL;
			old->__bits[1] &= ~0x3UL;
		}
	}
	return ret;
}
