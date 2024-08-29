#include <ctype.h>
#undef islower
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int islower(int c)
{
	return (unsigned)c-'a' < 26;
}

int __islower_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_islower), ICU_UCHAR_ISLOWER_SYMBOL);
		if (g_icu_opt_func.u_islower) {
			return g_icu_opt_func.u_islower(c);
		}
	}
#endif
	return islower(c);
}

weak_alias(__islower_l, islower_l);
