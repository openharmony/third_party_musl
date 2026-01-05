#include <stdint.h>
#include <stdio.h>
#include "mtest.h"
#include "functionalext.h"

static struct dd_d t[] = {
#include "crlibm/pow.h"
#include "ucb/pow.h"
#include "sanity/pow.h"
#include "special/pow.h"

};

/*
 * @tc.name      : test_pow_0100
 * @tc.desc      : Verify pow function exception handling and precision
 * @tc.level     : Level 0
 */
void test_pow_0100(void)
{
	#pragma STDC FENV_ACCESS ON
	double y;
	float d;
	int e, i, err = 0;
	struct dd_d *p;

	for (i = 0; i < sizeof t/sizeof *t; i++) {
		p = t + i;

		if (p->r < 0)
			continue;
		fesetround(p->r);
		feclearexcept(FE_ALL_EXCEPT);
		y = pow(p->x, p->x2);
		e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

		if (!checkexcept(e, p->e, p->r)) {
			if (fabs(y) < 0x1p-1022 && (e|INEXACT) == (INEXACT|UNDERFLOW))
				printf("X ");
			else
				err++;
			printf("%s:%d: bad fp exception: %s pow(%a,%a)=%a, want %s",
				p->file, p->line, rstr(p->r), p->x, p->x2, p->y, estr(p->e));
			printf(" got %s\n", estr(e));
		}
		d = ulperr(y, p->y, p->dy);
		if (!checkulp(d, p->r)) {
			printf("%s:%d: %s pow(%a,%a) want %a got %a ulperr %.3f = %a + %a\n",
				p->file, p->line, rstr(p->r), p->x, p->x2, p->y, y, d, d-p->dy, p->dy);
			err++;
		}
	}
    EXPECT_EQ("test_pow_0100", err, 0);
}

#ifdef MUSL_EXTERNAL_FUNCTION
/*
 * @tc.name      : test_pow_finite_0100
 * @tc.desc      : Verify __pow_finite function exception handling and precision
 * @tc.level     : Level 0
 */
void test_pow_finite_0100(void)
{
    #pragma STDC FENV_ACCESS ON
    double y;
    float d;
    int e, i, err = 0;
    struct dd_d *p;

    for (i = 0; i < sizeof t/sizeof *t; i++) {
        p = t + i;

        if (p->r < 0)
            continue;
        fesetround(p->r);
        feclearexcept(FE_ALL_EXCEPT);
        y = __pow_finite(p->x, p->x2);
        e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

        if (!checkexcept(e, p->e, p->r)) {
            if (fabs(y) < 0x1p-1022 && (e|INEXACT) == (INEXACT|UNDERFLOW))
                printf("X ");
            else
                err++;
            printf("%s:%d: bad fp exception: %s __pow_finite(%a,%a)=%a, want %s",
                p->file, p->line, rstr(p->r), p->x, p->x2, p->y, estr(p->e));
            printf(" got %s\n", estr(e));
        }
        d = ulperr(y, p->y, p->dy);
        if (!checkulp(d, p->r)) {
            printf("%s:%d: %s __pow_finite(%a,%a) want %a got %a ulperr %.3f = %a + %a\n",
                p->file, p->line, rstr(p->r), p->x, p->x2, p->y, y, d, d-p->dy, p->dy);
            err++;
        }
    }
    EXPECT_EQ("test_pow_finite_0100", err, 0);
}
#endif

int main(void)
{
    test_pow_0100();
#ifdef MUSL_EXTERNAL_FUNCTION
    test_pow_finite_0100();
#endif
    return 0;
}
