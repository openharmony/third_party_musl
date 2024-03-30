#include <stdlib.h>
#ifndef __LITEOS__
#include "param_check.h"
#endif

void *bsearch(const void *key, const void *base, size_t nel, size_t width, int (*cmp)(const void *, const void *))
{
	void *try;
	int sign;
#ifndef __LITEOS__
	PARAM_CHECK(key);
	PARAM_CHECK(base);
	PARAM_CHECK(cmp);
#endif
	while (nel > 0) {
		try = (char *)base + width*(nel/2);
		sign = cmp(key, try);
		if (sign < 0) {
			nel /= 2;
		} else if (sign > 0) {
			base = (char *)try + width;
			nel -= nel/2+1;
		} else {
			return try;
		}
	}
	return NULL;
}
