#include <ctype.h>
#undef isalpha
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int isalpha(int c)
{
	return ((unsigned)c|32)-'a' < 26;
}

int __isalpha_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_isalpha), ICU_UCHAR_ISALPHA_SYMBOL);
		if (g_icu_opt_func.u_isalpha) {
			return g_icu_opt_func.u_isalpha(c);
		}
	}
#endif
	return isalpha(c);
}

weak_alias(__isalpha_l, isalpha_l);
