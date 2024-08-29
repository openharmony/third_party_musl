#include <ctype.h>
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int tolower(int c)
{
	if (isupper(c)) return c | 32;
	return c;
}

int __tolower_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_tolower), ICU_UCHAR_TOLOWER_SYMBOL);
		if (g_icu_opt_func.u_tolower) {
			return g_icu_opt_func.u_tolower(c);
		}
	}
#endif
	return tolower(c);
}

weak_alias(__tolower_l, tolower_l);
