#include <string.h>

char *__stpncpy(char *dst, const char *src, size_t n)
{
	char *d = dst, *s = src;
	char *flag = dst;
	while (n > 0){
		*d = *src;
		if(*s != 0){
			s++;
			flag++;
		}
		d++;
		n--;
	}
	return (++flag);
}

weak_alias(__stpncpy, stpncpy);
