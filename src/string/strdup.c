#include <stdlib.h>
#include <string.h>

char *strdup(const char *s)
{
	size_t l = strlen(s);
	char *d = malloc(l+1);
	if (!d) return NULL;
	return memcpy(d, s, l+1);
}

#ifdef MUSL_EXTERNAL_FUNCTION
char *__strdup (const char *s)
{
	return strdup(s);
}
#endif