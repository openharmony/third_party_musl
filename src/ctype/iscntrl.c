#include <ctype.h>
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int iscntrl(int c)
{
	return (unsigned)c < 0x20 || c == 0x7f;
}

int __iscntrl_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_iscntrl), ICU_UCHAR_ISCNTRL_SYMBOL);
		if (g_icu_opt_func.u_iscntrl) {
			return g_icu_opt_func.u_iscntrl(c);
		}
	}
#endif
	return iscntrl(c);
}

weak_alias(__iscntrl_l, iscntrl_l);
