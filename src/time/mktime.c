#include "time_impl.h"
#include <errno.h>

static long long tm_local_secs(const struct tm *tm)
{
	struct tm tmp = *tm;

	return __tm_to_secs(&tmp);
}

static int tm_fields_match(const struct tm *a, const struct tm *b)
{
	return a->tm_sec == b->tm_sec &&
		a->tm_min == b->tm_min &&
		a->tm_hour == b->tm_hour &&
		a->tm_mday == b->tm_mday &&
		a->tm_mon == b->tm_mon &&
		a->tm_year == b->tm_year;
}

time_t mktime(struct tm *tm)
{
	struct tm new;
	long opp;
	long long t = __tm_to_secs(tm);

	__secs_to_zone(t, 1, &new.tm_isdst, &new.__tm_gmtoff, &opp, &new.__tm_zone, TZ_USE_ENV);

	if (tm->tm_isdst>=0 && new.tm_isdst!=tm->tm_isdst)
		t -= opp - new.__tm_gmtoff;

	t -= new.__tm_gmtoff;
	if ((time_t)t != t) goto error;

	__secs_to_zone(t, 0, &new.tm_isdst, &new.__tm_gmtoff, &opp, &new.__tm_zone, TZ_USE_ENV);

	if (__secs_to_tm(t + new.__tm_gmtoff, &new) < 0) goto error;

	*tm = new;
	return t;

error:
	errno = EOVERFLOW;
	return -1;
}

time_t mktime_noenv(struct tm *tm)
{
	struct tm want;
	struct tm new;
	struct tm alt_tm;
	long opp;
	long alt_opp;
	long long t = __tm_to_secs(tm);
	long long alt_t;
	long long want_t = t;

	__secs_to_zone(t, 1, &new.tm_isdst, &new.__tm_gmtoff, &opp, &new.__tm_zone, TZ_NO_USE_ENV);
	alt_opp = opp;

	if (tm->tm_isdst>=0 && new.tm_isdst!=tm->tm_isdst)
		t -= opp - new.__tm_gmtoff;

	t -= new.__tm_gmtoff;
	if ((time_t)t != t) goto error;

	__secs_to_zone(t, 0, &new.tm_isdst, &new.__tm_gmtoff, &opp, &new.__tm_zone, TZ_NO_USE_ENV);

	if (__secs_to_tm(t + new.__tm_gmtoff, &new) < 0) goto error;

	if (__secs_to_tm(want_t, &want) < 0) goto error;

	if (tm->tm_isdst < 0 && !tm_fields_match(&new, &want)) {
		alt_t = want_t - alt_opp;
		if ((time_t)alt_t != alt_t) goto error;
		__secs_to_zone(alt_t, 0, &alt_tm.tm_isdst, &alt_tm.__tm_gmtoff,
			&opp, &alt_tm.__tm_zone, TZ_NO_USE_ENV);
		if (__secs_to_tm(alt_t + alt_tm.__tm_gmtoff, &alt_tm) < 0) goto error;
		if (!tm_fields_match(&alt_tm, &new)) {
			if (tm_local_secs(&alt_tm) > tm_local_secs(&new)) {
				t = alt_t;
				new = alt_tm;
			}
		}
	}

	*tm = new;
	return t;

error:
	errno = EOVERFLOW;
	return -1;
}
