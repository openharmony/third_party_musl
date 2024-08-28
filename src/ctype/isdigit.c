#include <ctype.h>
#undef isdigit
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int isdigit(int c)
{
	return (unsigned)c-'0' < 10;
}

int __isdigit_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_isdigit), ICU_UCHAR_ISDIGIT_SYMBOL);
		if (g_icu_opt_func.u_isdigit) {
			return g_icu_opt_func.u_isdigit(c);
		}
	}
#endif
	return isdigit(c);
}

weak_alias(__isdigit_l, isdigit_l);
