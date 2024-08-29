#include <ctype.h>
#undef isupper
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int isupper(int c)
{
	return (unsigned)c-'A' < 26;
}

int __isupper_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_isupper), ICU_UCHAR_ISUPPER_SYMBOL);
		if (g_icu_opt_func.u_isupper) {
			return g_icu_opt_func.u_isupper(c);
		}
	}
#endif
	return isupper(c);
}

weak_alias(__isupper_l, isupper_l);
