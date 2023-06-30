#include "stdio_impl.h"
#include <string.h>

#define MIN(a,b) ((a)<(b) ? (a) : (b))

char *fgets(char *restrict s, int n, FILE *restrict f)
{
	char *p = s;
	unsigned char *z;
	size_t k;
	int c;

	FLOCK(f);

	if (n--<=1) {
		f->mode |= f->mode-1;
		FUNLOCK(f);
		if (n) {
			return NULL;
		}
		*s = '\0';
		return s;
	}

	while (n) {
		if (f->rpos == f->rend && __fill_buffer(f)) {
			if (p == s || !feof(f)) {
				s = 0;
			}
			break;
		}

		/* search minimal length */
		k = MIN(f->rend - f->rpos, n);
		z = memchr(f->rpos, '\n', k);
		if (z != NULL) {
			k = z - f->rpos + 1;
		}
		memcpy(p, f->rpos, k);
		f->rpos += k;
		p += k;
		n -= k;
		if (z || !n) {
			break;
		}
	}
	if (s) {
		*p = 0;
	}

	FUNLOCK(f);

	return s;
}

weak_alias(fgets, fgets_unlocked);
