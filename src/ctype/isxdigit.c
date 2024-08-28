#include <ctype.h>
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int isxdigit(int c)
{
	return isdigit(c) || ((unsigned)c|32)-'a' < 6;
}

int __isxdigit_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_isxdigit), ICU_UCHAR_ISXDIGIT_SYMBOL);
		if (g_icu_opt_func.u_isxdigit) {
			return g_icu_opt_func.u_isxdigit(c);
		}
	}
#endif
	return isxdigit(c);
}

weak_alias(__isxdigit_l, isxdigit_l);
