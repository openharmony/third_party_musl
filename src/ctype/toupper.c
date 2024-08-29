#include <ctype.h>
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int toupper(int c)
{
	if (islower(c)) return c & 0x5f;
	return c;
}

int __toupper_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_toupper), ICU_UCHAR_TOUPPER_SYMBOL);
		if (g_icu_opt_func.u_toupper) {
			return g_icu_opt_func.u_toupper(c);
		}
	}
#endif

	return toupper(c);
}

weak_alias(__toupper_l, toupper_l);
