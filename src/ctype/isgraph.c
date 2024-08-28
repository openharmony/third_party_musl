#include <ctype.h>
#undef isgraph
#ifdef FEATURE_ICU_LOCALE_TMP
#include <locale.h>
#include "locale_impl.h"
#endif

int isgraph(int c)
{
	return (unsigned)c-0x21 < 0x5e;
}

int __isgraph_l(int c, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE_TMP
	if (l && l->cat[LC_CTYPE] && l->cat[LC_CTYPE]->flag == ICU_VALID) {
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_isgraph), ICU_UCHAR_ISGRAPH_SYMBOL);
		if (g_icu_opt_func.u_isgraph) {
			return g_icu_opt_func.u_isgraph(c);
		}
	}
#endif
	return isgraph(c);
}

weak_alias(__isgraph_l, isgraph_l);
