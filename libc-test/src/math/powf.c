#include <stdint.h>
#include <stdio.h>
#include "mtest.h"
#include "functionalext.h"

static struct ff_f t[] = {
#include "ucb/powf.h"
#include "sanity/powf.h"
#include "special/powf.h"

};

/*
 * @tc.name      : test_powf_0100
 * @tc.desc      : Verify powf function exception handling and precision
 * @tc.level     : Level 0
 */
static void test_powf_0100(void)
{
	#pragma STDC FENV_ACCESS ON
	float y;
	float d;
	int e, i, err = 0;
	struct ff_f *p;

	for (i = 0; i < sizeof t/sizeof *t; i++) {
		p = t + i;

		if (p->r < 0)
			continue;
		fesetround(p->r);
		feclearexcept(FE_ALL_EXCEPT);
		y = powf(p->x, p->x2);
		e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

#ifndef __aarch64__
		if (!checkexcept(e, p->e, p->r)) {
			if (fabsf(y) < 0x1p-126f && (e|INEXACT) == (INEXACT|UNDERFLOW))
				printf("X ");
			else
				err++;
			printf("%s:%d: bad fp exception: %s powf(%a,%a)=%a, want %s",
				p->file, p->line, rstr(p->r), p->x, p->x2, p->y, estr(p->e));
			printf(" got %s\n", estr(e));
		}
#endif
		d = ulperrf(y, p->y, p->dy);
		if (!checkulp(d, p->r)) {
			printf("%s:%d: %s powf(%a,%a) want %a got %a ulperr %.3f = %a + %a\n",
				p->file, p->line, rstr(p->r), p->x, p->x2, p->y, y, d, d-p->dy, p->dy);
			err++;
		}
	}
    EXPECT_EQ("test_powf_0100", err, 0);
}

#ifdef MUSL_EXTERNAL_FUNCTION
/*
 * @tc.name      : test_powf_finite_0100
 * @tc.desc      : Verify __powf_finite function exception handling and precision
 * @tc.level     : Level 0
 */
static void test_powf_finite_0100(void)
{
    #pragma STDC FENV_ACCESS ON
    float y;
    float d;
    int e, i, err = 0;
    struct ff_f *p;

    for (i = 0; i < sizeof t/sizeof *t; i++) {
        p = t + i;

        if (p->r < 0)
            continue;
        fesetround(p->r);
        feclearexcept(FE_ALL_EXCEPT);
        y = __powf_finite(p->x, p->x2);
        e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

#ifndef __aarch64__
        if (!checkexcept(e, p->e, p->r)) {
            if (fabsf(y) < 0x1p-126f && (e|INEXACT) == (INEXACT|UNDERFLOW))
                printf("X ");
            else
                err++;
            printf("%s:%d: bad fp exception: %s __powf_finite(%a,%a)=%a, want %s",
                p->file, p->line, rstr(p->r), p->x, p->x2, p->y, estr(p->e));
            printf(" got %s\n", estr(e));
        }
#endif
        d = ulperrf(y, p->y, p->dy);
        if (!checkulp(d, p->r)) {
            printf("%s:%d: %s __powf_finite(%a,%a) want %a got %a ulperr %.3f = %a + %a\n",
                p->file, p->line, rstr(p->r), p->x, p->x2, p->y, y, d, d-p->dy, p->dy);
            err++;
        }
    }
    EXPECT_EQ("test_powf_finite_0100", err, 0);
}
#endif

int main(void)
{
    test_powf_0100();
#ifdef MUSL_EXTERNAL_FUNCTION
    test_powf_finite_0100();
#endif
    return 0;
}
