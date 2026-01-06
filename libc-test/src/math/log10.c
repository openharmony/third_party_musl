#include <stdint.h>
#include <stdio.h>
#include "mtest.h"
#include "functionalext.h"

static struct d_d t[] = {
#include "crlibm/log10.h"
#include "ucb/log10.h"
#include "sanity/log10.h"
#include "special/log10.h"

};

/*
 * @tc.name      : test_log10_0100
 * @tc.desc      : Verify log10 function exception handling and precision
 * @tc.level     : Level 0
 */
void test_log10_0100(void)
{
	#pragma STDC FENV_ACCESS ON
	double y;
	float d;
	int e, i, err = 0;
	struct d_d *p;

	for (i = 0; i < sizeof t/sizeof *t; i++) {
		p = t + i;

		if (p->r < 0)
			continue;
		fesetround(p->r);
		feclearexcept(FE_ALL_EXCEPT);
		y = log10(p->x);
		e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

		if (!checkexcept(e, p->e, p->r)) {
			printf("%s:%d: bad fp exception: %s log10(%a)=%a, want %s",
				p->file, p->line, rstr(p->r), p->x, p->y, estr(p->e));
			printf(" got %s\n", estr(e));
			err++;
		}
		d = ulperr(y, p->y, p->dy);
		if (!checkulp(d, p->r)) {
			printf("%s:%d: %s log10(%a) want %a got %a ulperr %.3f = %a + %a\n",
				p->file, p->line, rstr(p->r), p->x, p->y, y, d, d-p->dy, p->dy);
			err++;
		}
	}
    EXPECT_EQ("test_log10_0100", err, 0);
}

#ifdef MUSL_EXTERNAL_FUNCTION
/*
 * @tc.name      : test_log10_finite_0100
 * @tc.desc      : Verify __log10_finite function exception handling and precision
 * @tc.level     : Level 0
 */
void test_log10_finite_0100(void)
{
    #pragma STDC FENV_ACCESS ON
    double y;
    float d;
    int e, i, err = 0;
    struct d_d *p;

    for (i = 0; i < sizeof t/sizeof *t; i++) {
        p = t + i;

        if (p->r < 0)
            continue;
        fesetround(p->r);
        feclearexcept(FE_ALL_EXCEPT);
        y = __log10_finite(p->x);
        e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

        if (!checkexcept(e, p->e, p->r)) {
            printf("%s:%d: bad fp exception: %s __log10_finite(%a)=%a, want %s",
                p->file, p->line, rstr(p->r), p->x, p->y, estr(p->e));
            printf(" got %s\n", estr(e));
            err++;
        }
        d = ulperr(y, p->y, p->dy);
        if (!checkulp(d, p->r)) {
            printf("%s:%d: %s __log10_finite(%a) want %a got %a ulperr %.3f = %a + %a\n",
                p->file, p->line, rstr(p->r), p->x, p->y, y, d, d-p->dy, p->dy);
            err++;
        }
    }
    EXPECT_EQ("test_log10_finite_0100", err, 0);
}
#endif

int main(void)
{
    test_log10_0100();
#ifdef MUSL_EXTERNAL_FUNCTION
    test_log10_finite_0100();
#endif
    return 0;
}
