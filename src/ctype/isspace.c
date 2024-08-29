#include <ctype.h>
#undef isspace
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int isspace(int c)
{
	return c == ' ' || (unsigned)c-'\t' < 5;
}

int __isspace_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_isspace), ICU_UCHAR_ISSPACE_SYMBOL);
		if (g_icu_opt_func.u_isspace) {
			return g_icu_opt_func.u_isspace(c);
		}
	}
#endif
	return isspace(c);
}

weak_alias(__isspace_l, isspace_l);
