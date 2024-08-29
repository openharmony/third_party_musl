#include <ctype.h>
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int isblank(int c)
{
	return (c == ' ' || c == '\t');
}

int __isblank_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_isblank), ICU_UCHAR_ISBLANK_SYMBOL);
		if (g_icu_opt_func.u_isblank) {
			return g_icu_opt_func.u_isblank(c);
		}
	}
#endif
	return isblank(c);
}

weak_alias(__isblank_l, isblank_l);
