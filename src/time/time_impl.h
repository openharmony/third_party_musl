#ifndef __TIME_IMPL_H__
#define __TIME_IMPL_H__

#include <time.h>

// It will not call getenv to get the value of "TZ".
#define TZ_NO_USE_ENV 0
#define TZ_USE_ENV 1

hidden int __days_in_month(int, int);
hidden int __month_to_secs(int, int);
hidden long long __year_to_secs(long long, int *);
hidden long long __tm_to_secs(const struct tm *);
hidden const char *__tm_to_tzname(const struct tm *);
hidden int __secs_to_tm(long long, struct tm *);
hidden void __secs_to_zone(long long, int, int *, long *, long *, const char **, int);
hidden const char *__strftime_fmt_1(char (*)[100], size_t *, int, const struct tm *, locale_t, int);
extern hidden const char __utc[];
extern hidden const char __gmt[];

#endif
