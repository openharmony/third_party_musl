#include "pthread_impl.h"
#if defined(__LITEOS__) || defined(__HISPARK_LINUX__)
int pthread_setcanceltype(int new, int *old)
{
#ifdef FEATURE_PTHREAD_CANCEL
	struct pthread *self = __pthread_self();
	if (new > 1U) return EINVAL;
	if (old) *old = self->cancelasync;
	self->cancelasync = new;
	if (new) pthread_testcancel();
	return 0;
#endif
}
#else
#define NOT_ENABLE (1)

int pthread_setcanceltype(int new, int *old)
{
#if NOT_ENABLE == 0
#ifdef FEATURE_PTHREAD_CANCEL
	struct pthread *self = __pthread_self();
	if (new > 1U) return EINVAL;
	if (old) *old = self->cancelasync;
	self->cancelasync = new;
	if (new) pthread_testcancel();
	return 0;
#endif
#endif
}
#endif
