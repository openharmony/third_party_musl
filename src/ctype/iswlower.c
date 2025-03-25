#include <wctype.h>
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
#include <string>
#include "locale_impl.h"
#endif
#endif

int iswlower(wint_t wc)
{
	return towupper(wc) != wc;
}

int __iswlower_l(wint_t c, locale_t l)
{
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
	if (icu_locale_wctype_enable && l && l->cat[LC_CTYPE]
		&& l->cat[LC_CTYPE]->flag == ICU_VALID) {
		char* type_name = (char*)(l->cat[LC_CTYPE]->name);
		if (!strcmp(type_name, "zh_CN") || !strcmp(type_name, "en_US.UTF-8")) {
			return g_icu_opt_func.u_islower(c);
		}
	}
#endif
#endif
	return iswlower(c);
}

weak_alias(__iswlower_l, iswlower_l);
