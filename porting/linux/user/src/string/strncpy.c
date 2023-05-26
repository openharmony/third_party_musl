#include <string.h>

char *strncpy(char *dst, const char *src, size_t n)
{
	char *d = dst, *s = src;
	while (n > 0){
		*d = *s;
		if(*s != 0){
			s++;
		}
		d++;
		n--;
	}
	return (d);
}