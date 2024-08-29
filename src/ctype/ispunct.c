#include <ctype.h>
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int ispunct(int c)
{
	return isgraph(c) && !isalnum(c);
}

int __ispunct_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_isgraph), ICU_UCHAR_ISGRAPH_SYMBOL);
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_isalnum), ICU_UCHAR_ISALNUM_SYMBOL);
		if (g_icu_opt_func.u_isgraph && g_icu_opt_func.u_isalnum) {
			return g_icu_opt_func.u_isgraph(c) && !g_icu_opt_func.u_isalnum(c);
		}
	}
#endif
	return ispunct(c);
}

weak_alias(__ispunct_l, ispunct_l);
