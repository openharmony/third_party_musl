#define _GNU_SOURCE
#include "pthread_impl.h"

#if defined(MUSL_EXTERNAL_FUNCTION) || defined(PTHREAD_CANCEL_IN_STATIC_LIB) || \
	defined(__LITEOS__) || defined(__HISPARK_LINUX__)
static void dummy()
{
}

weak_alias(dummy, __testcancel);

void __pthread_testcancel()
{
#ifdef MUSL_EXTERNAL_FUNCTION
	if (get_pthread_extended_function_policy() == 0) {
		return;
	}
#endif
	__testcancel();
}

weak_alias(__pthread_testcancel, pthread_testcancel);
#else

static void dummy()
{
}

weak_alias(dummy, __testcancel);

hidden void __pthread_testcancel()
{
	__testcancel();
}
#endif
