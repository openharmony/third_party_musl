#include <ctype.h>
#undef isprint
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int isprint(int c)
{
	return (unsigned)c-0x20 < 0x5f;
}

int __isprint_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_isprint), ICU_UCHAR_ISPRINT_SYMBOL);
		if (g_icu_opt_func.u_isprint) {
			return g_icu_opt_func.u_isprint(c);
		}
	}
#endif
	return isprint(c);
}

weak_alias(__isprint_l, isprint_l);
