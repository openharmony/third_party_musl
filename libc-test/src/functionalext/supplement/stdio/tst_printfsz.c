/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

/*
 * Ported from the reference C library test suite (tst-printfsz); the
 * test expectations are verbatim from the reference. The printf_size
 * handler is reimplemented with public snprintf.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <printf.h>
#include "functionalext.h"

#define V       12345678.12345678

/* Reimplementation of printf_size semantics for the units conversions,
 * using public snprintf for the numeric part. */
static int my_printf_size(FILE *fp, const struct printf_info *info,
                       const void *const *args)
{
	static const char binary_units[] = " kmgtpezy";
	static const char decimal_units[] = " KMGTPEZY";
	int upper = isupper((int)info->spec) != 0;
	const char *tag = upper ? decimal_units : binary_units;
	int divisor = upper ? 1000 : 1024;

	int is_ldbl = info->is_long_double != 0;
	long double lval = 0;
	double val = 0;
	int is_neg = 0;
	const char *special = NULL;

	if (is_ldbl) {
		lval = *(const long double *)args[0];
		if (isnan(lval)) {
			special = "nan";
		} else if (isinf(lval)) {
			is_neg = signbit(lval);
			special = "inf";
		} else {
			while (lval >= divisor && tag[1] != '\0') {
				lval /= divisor;
				tag++;
			}
		}
	} else {
		val = *(const double *)args[0];
		if (isnan(val)) {
			special = "nan";
		} else if (isinf(val)) {
			is_neg = signbit(val);
			special = "inf";
		} else {
			while (val >= divisor && tag[1] != '\0') {
				val /= divisor;
				tag++;
			}
		}
	}

	int done = 0;
	if (special) {
		int width = info->prec > info->width ? info->prec : info->width;
		if (is_neg || info->showsign || info->space) {
			width--;
		}
		width -= 3;
		if (!info->left && width > 0) {
			for (int i = 0; i < width; i++) {
				if (fputc(' ', fp) == EOF) {
					return -1;
				}
				done++;
			}
		}
		int c = is_neg ? '-' : (info->showsign ? '+' :
		                        (info->space ? ' ' : 0));
		if (c && fputc(c, fp) == EOF) {
			return -1;
		}
		if (c) {
			done++;
		}
		if (fwrite(special, 1, 3, fp) != 3) {
			return -1;
		}
		done += 3;
		if (info->left && width > 0) {
			for (int i = 0; i < width; i++) {
				if (fputc(' ', fp) == EOF) {
					return -1;
				}
				done++;
			}
		}
		return done;
	}

	int prec = info->prec < 0 ? 3 : info->prec;
	char nbuf[64];
	int n;

	if (info->left && info->pad != '0') {
		/* Emit the number, the unit character, then right padding. */
		if (is_ldbl) {
			n = snprintf(nbuf, sizeof(nbuf), "%.*Lf", prec, lval);
		} else {
			n = snprintf(nbuf, sizeof(nbuf), "%.*f", prec, val);
		}
		if (n < 0) {
			return -1;
		}
		if (fwrite(nbuf, 1, (size_t)n, fp) != (size_t)n) {
			return -1;
		}
		done = n;
		if (fputc(*tag, fp) == EOF) {
			return -1;
		}
		done++;
		if (info->width > done) {
			for (int i = 0; i < info->width - done; i++) {
				if (fputc(' ', fp) == EOF) {
					return -1;
				}
			}
			done = info->width;
		}
		return done;
	}

	/* Let snprintf handle width/zero-pad/flags; reserve one column
	 * for the unit character. */
	char fbuf[16];
	char *f = fbuf;
	*f++ = '%';
	if (info->showsign) {
		*f++ = '+';
	}
	if (info->space) {
		*f++ = ' ';
	}
	if (info->alt) {
		*f++ = '#';
	}
	if (info->pad == '0') {
		*f++ = '0';
	}
	*f++ = '*';
	*f++ = '.';
	*f++ = '*';
	*f++ = 'f';
	*f = '\0';

	int w = info->width > 0 ? info->width - 1 : 0;
	if (is_ldbl) {
		n = snprintf(nbuf, sizeof(nbuf), fbuf, w, prec, lval);
	} else {
		n = snprintf(nbuf, sizeof(nbuf), fbuf, w, prec, val);
	}
	if (n < 0) {
		return -1;
	}
	if (fwrite(nbuf, 1, (size_t)n, fp) != (size_t)n) {
		return -1;
	}
	if (fputc(*tag, fp) == EOF) {
		return -1;
	}
	return n + 1;
}

static int my_printf_size_info(const struct printf_info *info, size_t n, int *argtypes, int *size)
{
	/* We need only one double or long double argument. */
	if (n >= 1) {
		argtypes[0] = PA_DOUBLE |
		              (info->is_long_double ? PA_FLAG_LONG_DOUBLE : 0);
		size[0] = info->is_long_double ? sizeof(long double) : sizeof(double);
	}
	return 1;
}

/**
 * @tc.name      : tst_printfsz_0100
 * @tc.desc      : reference tst-printfsz check 1 (basic units and precision)
 * @tc.level     : Level 0
 */
void tst_printfsz_0100(void)
{
	char buf[1024];
	int ret = snprintf(buf, sizeof(buf), "%g %b %B %.0b %.0B %.1b %.1B %8.0b %08.0B",
	                   V, 1025., V, V, V, V, V, V, V, V);
	EXPECT_TRUE("tst_printfsz_0100", ret > 0);
	EXPECT_STREQ("tst_printfsz_0100", buf,
	             "1.23457e+07 1.001k 12.346M 12m 12M 11.8m 12.3M      12m 0000012M");
}

/**
 * @tc.name      : tst_printfsz_0200
 * @tc.desc      : reference tst-printfsz check 2 (left adjustment)
 * @tc.level     : Level 0
 */
void tst_printfsz_0200(void)
{
	char buf[1024];
	int ret = snprintf(buf, sizeof(buf), "%b|%B|%-20.2b|%-10.0b|%-10.8b|%-10.2B|",
	                   V, V, V, V, V, V, V, V, V, V, V);
	EXPECT_TRUE("tst_printfsz_0200", ret > 0);
	EXPECT_STREQ("tst_printfsz_0200", buf,
	             "11.774m|12.346M|11.77m              |12m       |11.77375614m|12.35M    |");
}

/**
 * @tc.name      : tst_printfsz_0300
 * @tc.desc      : reference tst-printfsz check 3 (alt form and star width/precision)
 * @tc.level     : Level 0
 */
void tst_printfsz_0300(void)
{
	char buf[1024];
	int ret = snprintf(buf, sizeof(buf), "%#.0B %*.0b %10.*b %*.*B %10.2B",
	                   V, 2, V, 2, V, 10, 2, V, V);
	EXPECT_TRUE("tst_printfsz_0300", ret > 0);
	EXPECT_STREQ("tst_printfsz_0300", buf,
	             "12.M 12m     11.77m     12.35M     12.35M");
}

/**
 * @tc.name      : tst_printfsz_0400
 * @tc.desc      : reference tst-printfsz check 4 (unit boundaries)
 * @tc.level     : Level 0
 */
void tst_printfsz_0400(void)
{
	char buf[1024];
	int ret = snprintf(buf, sizeof(buf), "%6B %6.1B %b %B %b %B",
	                   V, V, 1000.0, 1000.0, 1024.0, 1024.0);
	EXPECT_TRUE("tst_printfsz_0400", ret > 0);
	EXPECT_STREQ("tst_printfsz_0400", buf,
	             "12.346M  12.3M 1000.000  1.000K 1.000k 1.024K");
}

int main(int argc, char *argv[])
{
	int ret1 = register_printf_specifier('b', my_printf_size, my_printf_size_info);
	int ret2 = register_printf_specifier('B', my_printf_size, my_printf_size_info);
	EXPECT_EQ("tst_printfsz", ret1, 0);
	EXPECT_EQ("tst_printfsz", ret2, 0);

	tst_printfsz_0100();
	tst_printfsz_0200();
	tst_printfsz_0300();
	tst_printfsz_0400();
	return t_status;
}
