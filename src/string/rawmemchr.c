#include <string.h>

void *__rawmemchr(const void *s, int c)
{
    return memchr(s, c, (size_t)-1);
}
