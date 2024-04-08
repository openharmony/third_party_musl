#include <locale.h>
#include <limits.h>
#include "libc.h"
// #include <musl_log.h>
// #include <stdio.h>
#include "locale_impl.h"

static const struct lconv posix_lconv = {
	.decimal_point = ".",
	.thousands_sep = "",
	.grouping = "",
	.int_curr_symbol = "",
	.currency_symbol = "",
	.mon_decimal_point = "",
	.mon_thousands_sep = "",
	.mon_grouping = "",
	.positive_sign = "",
	.negative_sign = "",
	.int_frac_digits = CHAR_MAX,
	.frac_digits = CHAR_MAX,
	.p_cs_precedes = CHAR_MAX,
	.p_sep_by_space = CHAR_MAX,
	.n_cs_precedes = CHAR_MAX,
	.n_sep_by_space = CHAR_MAX,
	.p_sign_posn = CHAR_MAX,
	.n_sign_posn = CHAR_MAX,
	.int_p_cs_precedes = CHAR_MAX,
	.int_p_sep_by_space = CHAR_MAX,
	.int_n_cs_precedes = CHAR_MAX,
	.int_n_sep_by_space = CHAR_MAX,
	.int_p_sign_posn = CHAR_MAX,
	.int_n_sign_posn = CHAR_MAX,
};

struct lconv *localeconv(void)
{
	// printf("[wzx_locale_conv] libc.global_locale.cat[0]->name %s\n", libc.global_locale.cat[0]->name);
	// printf("[wzx_locale_conv] libc.global_locale.cat[1]->name %s\n", libc.global_locale.cat[1]->name);
	// printf("[wzx_locale_conv] libc.global_locale.cat[LC_NUMERIC]->name %s\n", libc.global_locale.cat[LC_NUMERIC]->name);

	return (void *)&posix_lconv;
}
