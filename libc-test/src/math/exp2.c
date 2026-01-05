#include <stdint.h>
#include <stdio.h>
#include "mtest.h"
#include "functionalext.h"

static struct d_d t[] = {
#include "sanity/exp2.h"
#include "special/exp2.h"

};

/*
 * @tc.name      : test_exp2_0100
 * @tc.desc      : Verify exp2 function exception handling and precision
 * @tc.level     : Level 0
 */
static void test_exp2_0100(void)
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
		y = exp2(p->x);
		e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

		if (!checkexcept(e, p->e, p->r)) {
			if (fabs(y) < 0x1p-1022 && (e|INEXACT) == (INEXACT|UNDERFLOW))
				printf("X ");
			else
				err++;
			printf("%s:%d: bad fp exception: %s exp2(%a)=%a, want %s",
				p->file, p->line, rstr(p->r), p->x, p->y, estr(p->e));
			printf(" got %s\n", estr(e));
		}
		d = ulperr(y, p->y, p->dy);
		if (!checkulp(d, p->r)) {
			printf("%s:%d: %s exp2(%a) want %a got %a ulperr %.3f = %a + %a\n",
				p->file, p->line, rstr(p->r), p->x, p->y, y, d, d-p->dy, p->dy);
			err++;
		}
	}
    EXPECT_EQ("test_exp2_0100", err, 0);
}

#ifdef MUSL_EXTERNAL_FUNCTION
/*
 * @tc.name      : test_exp2_finite_0100
 * @tc.desc      : Verify __exp2_finite function exception handling and precision
 * @tc.level     : Level 0
 */
static void test_exp2_finite_0100(void)
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
        y = __exp2_finite(p->x);
        e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

        if (!checkexcept(e, p->e, p->r)) {
            if (fabs(y) < 0x1p-1022 && (e|INEXACT) == (INEXACT|UNDERFLOW))
                printf("X ");
            else
                err++;
            printf("%s:%d: bad fp exception: %s __exp2_finite(%a)=%a, want %s",
                p->file, p->line, rstr(p->r), p->x, p->y, estr(p->e));
            printf(" got %s\n", estr(e));
        }
        d = ulperr(y, p->y, p->dy);
        if (!checkulp(d, p->r)) {
            printf("%s:%d: %s __exp2_finite(%a) want %a got %a ulperr %.3f = %a + %a\n",
                p->file, p->line, rstr(p->r), p->x, p->y, y, d, d-p->dy, p->dy);
            err++;
        }
    }
    EXPECT_EQ("test_exp2_finite_0100", err, 0);
}
#endif

int main(void)
{
    test_exp2_0100();
#ifdef MUSL_EXTERNAL_FUNCTION
    test_exp2_finite_0100();
#endif
    return 0;
}
