#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int __vasprintf(char **s, const char *fmt, va_list ap, unsigned int mode_flags)
{
	va_list ap2;
	va_copy(ap2, ap);
	int l = __vsnprintf(0, 0, fmt, ap2, mode_flags);
	va_end(ap2);

	if (l<0 || !(*s=malloc(l+1U))) return -1;
	return __vsnprintf(*s, l+1U, fmt, ap, mode_flags);
}

int vasprintf(char **s, const char *fmt, va_list ap)
{
	return __vasprintf(s, fmt, ap, 0);
}
