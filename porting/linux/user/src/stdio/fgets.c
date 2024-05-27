#include "stdio_impl.h"
#include <string.h>
#include "param_check.h"

#define MIN(a,b) ((a)<(b) ? (a) : (b))
char *fgets(char *restrict s, int n, FILE *restrict f)
{
	char *p = s;
	unsigned char *z;
	size_t k;
	int c;
	PARAM_CHECK(f);

	FLOCK(f);

	if (n--<=1) {
		f->mode |= f->mode-1;
		FUNLOCK(f);
		if (n) return 0;
		*s = 0;
		return s;
	}

	while (n) {
		if (f->rpos != f->rend) {
			z = memchr(f->rpos, '\n', MIN(f->rend - f->rpos, n));
			k = z ? z - f->rpos + 1 : f->rend - f->rpos;
			k = MIN(k, n);
			memcpy(p, f->rpos, k);
			f->rpos += k;
			p += k;
			n -= k;
			if (z || !n) break;
		}
		if ((c = getc_unlocked(f)) < 0) {
			if (p==s || !feof(f)) s = 0;
			break;
		}
		n--;
		if ((*p++ = c) == '\n') break;
	}
	if (s) *p = 0;

	FUNLOCK(f);

	return s;
}


weak_alias(fgets, fgets_unlocked);