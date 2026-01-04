/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _XOPEN_SOURCE 700
#include <stdint.h>
#include <stdio.h>
#include "mtest.h"
#include "functionalext.h"

static struct di_d t[] = {
#include "sanity/yn.h"
#include "special/yn.h"
};

/*
 * @tc.name      : test_ynl_0100
 * @tc.desc      : Verify ynl function exception handling and precision
 * @tc.level     : Level 0
 */
void test_ynl_0100(void)
{
	#pragma STDC FENV_ACCESS ON
	double y;
	float d;
	int e, i, err = 0;
	struct di_d *p;

	for (i = 0; i < sizeof t/sizeof *t; i++) {
		p = t + i;

		if (p->r < 0)
			continue;
		fesetround(p->r);
		feclearexcept(FE_ALL_EXCEPT);
		y = ynl(p->i, p->x);
		e = fetestexcept(INEXACT|INVALID|DIVBYZERO|UNDERFLOW|OVERFLOW);

#ifndef __aarch64__
		if (!checkexcept(e, p->e, p->r)) {
			printf("%s:%d: bad fp exception: %s ynl(%lld, %a)=%a, want %s",
				p->file, p->line, rstr(p->r), p->i, p->x, p->y, estr(p->e));
			printf(" got %s\n", estr(e));
			err++;
		}
#endif
		d = ulperr(y, p->y, p->dy);
		if ((!(p->x < 0) && !checkulp(d, p->r)) || (p->x < 0 && !isnan(y) && y != -inf)) {
			printf("%s:%d: %s ynl(%lld, %a) want %a got %a, ulperr %.3f = %a + %a\n",
				p->file, p->line, rstr(p->r), p->i, p->x, p->y, y, d, d-p->dy, p->dy);
			err++;
		}
	}
	EXPECT_EQ("test_ynl_0100", err, 0);
}

int main(void)
{
	test_ynl_0100();
	return 0;
}