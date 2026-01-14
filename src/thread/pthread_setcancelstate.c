#include "pthread_impl.h"
#if defined(__LITEOS__) || defined(__HISPARK_LINUX__)
int __pthread_setcancelstate(int new, int *old)
{
#ifdef FEATURE_PTHREAD_CANCEL
	if (new > 2U) return EINVAL;
	struct pthread *self = __pthread_self();
	if (old) *old = self->canceldisable;
	self->canceldisable = new;
	return 0;
#endif
}
#else
#define NOT_ENABLE (1)
int __pthread_setcancelstate(int new, int *old)
{
#if NOT_ENABLE == 0
#ifdef FEATURE_PTHREAD_CANCEL
	if (new > 2U) return EINVAL;
	struct pthread *self = __pthread_self();
	if (old) *old = self->canceldisable;
	self->canceldisable = new;
	return 0;
#endif
#endif
}
#endif

weak_alias(__pthread_setcancelstate, pthread_setcancelstate);
