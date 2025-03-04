#include <string.h>

char *strncpy(char *dst, const char *src, size_t n)
{
	char *d = dst, *s = (char *)src;
	while (n > 0) {
		if ((*d = *s) != 0) {
			s++;
		}
			d++;
			n--;
		}
	return (dst);
}
