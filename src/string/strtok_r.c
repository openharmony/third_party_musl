#include <string.h>

char *strtok_r(char *restrict s, const char *restrict sep, char **restrict p)
{
	if (!s && !(s = *p)) return NULL;
	s += strspn(s, sep);
	if (!*s) return *p = 0;
	*p = s + strcspn(s, sep);
	if (**p) *(*p)++ = 0;
	else *p = 0;
	return s;
}

#ifdef MUSL_EXTERNAL_FUNCTION
char *__strtok_r(char *restrict s, const char *restrict sep, char **restrict p)
{
	return strtok_r(s, sep, p);
}
#endif