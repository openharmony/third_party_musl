#include <stdint.h>
#include <stdio.h>
#include "mtest.h"
#include "functionalext.h"

static struct f_f t[] = {
#include "ucb/acosf.h"
#include "sanity/acosf.h"
#include "special/acosf.h"

};

/*
 * @tc.name      : test_acosf_0100
 * @tc.desc      : Verify acosf function exception handling and precision
 * @tc.level     : Level 0
 */
static void test_acosf_0100(void)
{
	#pragma STDC FENV_ACCESS ON
	float y;
	float d;
	int e, i, err = 0;
	struct f_f *p;

	for (i = 0; i < sizeof t/sizeof *t; i++) {
		p = t + i;

		if (p->r < 0)
			continue;
		fesetround(p->r);
		feclearexcept(FE_ALL_EXCEPT);
		y = acosf(p->x);
		e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

		if (!checkexcept(e, p->e, p->r)) {
			printf("%s:%d: bad fp exception: %s acosf(%a)=%a, want %s",
				p->file, p->line, rstr(p->r), p->x, p->y, estr(p->e));
			printf(" got %s\n", estr(e));
			err++;
		}
		d = ulperrf(y, p->y, p->dy);
		if (!checkulp(d, p->r)) {
			printf("%s:%d: %s acosf(%a) want %a got %a ulperr %.3f = %a + %a\n",
				p->file, p->line, rstr(p->r), p->x, p->y, y, d, d-p->dy, p->dy);
			err++;
		}
	}
    EXPECT_EQ("test_acosf_0100", err, 0);
}

#ifdef MUSL_EXTERNAL_FUNCTION
/*
 * @tc.name      : test_acosf_finite_0100
 * @tc.desc      : Verify __acosf_finite function exception handling and precision
 * @tc.level     : Level 0
 */
static void test_acosf_finite_0100(void)
{
    #pragma STDC FENV_ACCESS ON
    float y;
    float d;
    int e, i, err = 0;
    struct f_f *p;

    for (i = 0; i < sizeof t/sizeof *t; i++) {
        p = t + i;

        if (p->r < 0)
            continue;
        fesetround(p->r);
        feclearexcept(FE_ALL_EXCEPT);
        y = __acosf_finite(p->x);
        e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

        if (!checkexcept(e, p->e, p->r)) {
            printf("%s:%d: bad fp exception: %s __acosf_finite(%a)=%a, want %s",
                p->file, p->line, rstr(p->r), p->x, p->y, estr(p->e));
            printf(" got %s\n", estr(e));
            err++;
        }
        d = ulperrf(y, p->y, p->dy);
        if (!checkulp(d, p->r)) {
            printf("%s:%d: %s __acosf_finite(%a) want %a got %a ulperr %.3f = %a + %a\n",
                p->file, p->line, rstr(p->r), p->x, p->y, y, d, d-p->dy, p->dy);
            err++;
        }
    }
    EXPECT_EQ("test_acosf_finite_0100", err, 0);
}
#endif

int main(void)
{
    test_acosf_0100();
#ifdef MUSL_EXTERNAL_FUNCTION
    test_acosf_finite_0100();
#endif
    return 0;
}
