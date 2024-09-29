#include "time_impl.h"
#include <errno.h>
#include <limits.h>

struct tm *__localtime_r(const time_t *restrict t, struct tm *restrict tm)
{
	/* Reject time_t values whose year would overflow int because
	 * __secs_to_zone cannot safely handle them. */
	if (*t < INT_MIN * 31622400LL || *t > INT_MAX * 31622400LL) {
		errno = EOVERFLOW;
		return 0;
	}
	__secs_to_zone(*t, 0, &tm->tm_isdst, &tm->__tm_gmtoff, 0, &tm->__tm_zone, TZ_USE_ENV);
	if (__secs_to_tm((long long)*t + tm->__tm_gmtoff, tm) < 0) {
		errno = EOVERFLOW;
		return 0;
	}
	return tm;
}

// There is a problem with multi-threaded calls to setenv/getenv.
// So we provide this interface for those who don't care about the value of "TZ" set with setenv,
// then it will only get the value of "TZ" set by system, no need to call getenv("TZ").
struct tm *localtime_noenv_r(const time_t *restrict t, struct tm *restrict tm)
{
	/* Reject time_t values whose year would overflow int because
	 * __secs_to_zone cannot safely handle them. */
	if (*t < INT_MIN * 31622400LL || *t > INT_MAX * 31622400LL) {
		errno = EOVERFLOW;
		return 0;
	}
	__secs_to_zone(*t, 0, &tm->tm_isdst, &tm->__tm_gmtoff, 0, &tm->__tm_zone, TZ_NO_USE_ENV);
	if (__secs_to_tm((long long)*t + tm->__tm_gmtoff, tm) < 0) {
		errno = EOVERFLOW;
		return 0;
	}
	return tm;
}

weak_alias(__localtime_r, localtime_r);
