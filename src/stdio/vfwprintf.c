#include "stdio_impl.h"
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <inttypes.h>
#ifdef MUSL_EXTERNAL_FUNCTION
#include "printf_ext.h"
#endif

/* Convenient bit representation for modifier flags, which all fall
 * within 31 codepoints of the space character. */

#define ALT_FORM   (1U<<'#'-' ')
#define ZERO_PAD   (1U<<'0'-' ')
#define LEFT_ADJ   (1U<<'-'-' ')
#define PAD_POS    (1U<<' '-' ')
#define MARK_POS   (1U<<'+'-' ')
#define GROUPED    (1U<<'\''-' ')

#define FLAGMASK (ALT_FORM|ZERO_PAD|LEFT_ADJ|PAD_POS|MARK_POS|GROUPED)

/* State machine to accept length modifiers + conversion specifiers.
 * Result is 0 on failure, or an argument type to pop on success. */

enum {
	BARE, LPRE, LLPRE, HPRE, HHPRE, BIGLPRE,
	ZTPRE, JPRE,
	STOP,
	PTR, INT, UINT, ULLONG,
	LONG, ULONG,
	SHORT, USHORT, CHAR, UCHAR,
	LLONG, SIZET, IMAX, UMAX, PDIFF, UIPTR,
	DBL, LDBL,
	NOARG,
	MAXSTATE
};

#define S(x) [(x)-'A']

static const unsigned char states[]['z'-'A'+1] = {
	{ /* 0: bare types */
		S('d') = INT, S('i') = INT,
		S('o') = UINT, S('u') = UINT, S('x') = UINT, S('X') = UINT,
		S('e') = DBL, S('f') = DBL, S('g') = DBL, S('a') = DBL,
		S('E') = DBL, S('F') = DBL, S('G') = DBL, S('A') = DBL,
		S('c') = INT, S('C') = UINT,
		S('s') = PTR, S('S') = PTR, S('p') = UIPTR, S('n') = PTR,
		S('m') = NOARG,
		S('l') = LPRE, S('h') = HPRE, S('L') = BIGLPRE,
		S('z') = ZTPRE, S('j') = JPRE, S('t') = ZTPRE,
	}, { /* 1: l-prefixed */
		S('d') = LONG, S('i') = LONG,
		S('o') = ULONG, S('u') = ULONG, S('x') = ULONG, S('X') = ULONG,
		S('e') = DBL, S('f') = DBL, S('g') = DBL, S('a') = DBL,
		S('E') = DBL, S('F') = DBL, S('G') = DBL, S('A') = DBL,
		S('c') = UINT, S('s') = PTR, S('n') = PTR,
		S('l') = LLPRE,
	}, { /* 2: ll-prefixed */
		S('d') = LLONG, S('i') = LLONG,
		S('o') = ULLONG, S('u') = ULLONG,
		S('x') = ULLONG, S('X') = ULLONG,
		S('n') = PTR,
	}, { /* 3: h-prefixed */
		S('d') = SHORT, S('i') = SHORT,
		S('o') = USHORT, S('u') = USHORT,
		S('x') = USHORT, S('X') = USHORT,
		S('n') = PTR,
		S('h') = HHPRE,
	}, { /* 4: hh-prefixed */
		S('d') = CHAR, S('i') = CHAR,
		S('o') = UCHAR, S('u') = UCHAR,
		S('x') = UCHAR, S('X') = UCHAR,
		S('n') = PTR,
	}, { /* 5: L-prefixed */
		S('e') = LDBL, S('f') = LDBL, S('g') = LDBL, S('a') = LDBL,
		S('E') = LDBL, S('F') = LDBL, S('G') = LDBL, S('A') = LDBL,
		S('n') = PTR,
	}, { /* 6: z- or t-prefixed (assumed to be same size) */
		S('d') = PDIFF, S('i') = PDIFF,
		S('o') = SIZET, S('u') = SIZET,
		S('x') = SIZET, S('X') = SIZET,
		S('n') = PTR,
	}, { /* 7: j-prefixed */
		S('d') = IMAX, S('i') = IMAX,
		S('o') = UMAX, S('u') = UMAX,
		S('x') = UMAX, S('X') = UMAX,
		S('n') = PTR,
	}
};

#define OOB(x) ((unsigned)(x)-'A' > 'z'-'A')

union arg
{
	uintmax_t i;
	long double f;
	void *p;
};

static void pop_arg(union arg *arg, int type, va_list *ap)
{
	switch (type) {
	       case PTR:	arg->p = va_arg(*ap, void *);
	break; case INT:	arg->i = va_arg(*ap, int);
	break; case UINT:	arg->i = va_arg(*ap, unsigned int);
	break; case LONG:	arg->i = va_arg(*ap, long);
	break; case ULONG:	arg->i = va_arg(*ap, unsigned long);
	break; case ULLONG:	arg->i = va_arg(*ap, unsigned long long);
	break; case SHORT:	arg->i = (short)va_arg(*ap, int);
	break; case USHORT:	arg->i = (unsigned short)va_arg(*ap, int);
	break; case CHAR:	arg->i = (signed char)va_arg(*ap, int);
	break; case UCHAR:	arg->i = (unsigned char)va_arg(*ap, int);
	break; case LLONG:	arg->i = va_arg(*ap, long long);
	break; case SIZET:	arg->i = va_arg(*ap, size_t);
	break; case IMAX:	arg->i = va_arg(*ap, intmax_t);
	break; case UMAX:	arg->i = va_arg(*ap, uintmax_t);
	break; case PDIFF:	arg->i = va_arg(*ap, ptrdiff_t);
	break; case UIPTR:	arg->i = (uintptr_t)va_arg(*ap, void *);
	break; case DBL:	arg->f = va_arg(*ap, double);
	break; case LDBL:	arg->f = va_arg(*ap, long double);
	}
}

#ifdef MUSL_EXTERNAL_FUNCTION
/* Translate PA_* arginfo types to internal pop_arg types.
 * Returns the internal type, or -1 for an unknown type. */
static int pa_to_st(int at)
{
	/* ABI contract: any type carrying PA_FLAG_PTR is passed as a
	 * plain pointer (e.g. PA_INT|PA_FLAG_PTR is an int *). */
	if (at & PA_FLAG_PTR) {
		return PTR;
	}
	switch (at & ~PA_FLAG_MASK) {
	case PA_INT:
		if (at & PA_FLAG_LONG_LONG) {
			return LLONG;
		}
		if (at & PA_FLAG_LONG) {
			return LONG;
		}
		if (at & PA_FLAG_SHORT) {
			return SHORT;
		}
		return INT;
	case PA_CHAR:
		return CHAR;
	case PA_WCHAR:
		return INT;
	case PA_STRING:
	case PA_WSTRING:
	case PA_POINTER:
		return PTR;
	case PA_FLOAT:
	case PA_DOUBLE:
		if (at & PA_FLAG_LONG_DOUBLE) {
			return LDBL;
		}
		return DBL;
	default:
		return -1;
	}
}

/* Build the printf_info passed to the handler (wide stream). */
static void fill_printf_info(struct printf_info *info, unsigned char ch,
                             unsigned fl, int w, int p,
                             unsigned int user_mod, unsigned ps)
{
	__builtin_memset(info, 0, sizeof(*info));
	/* ABI contract: -1 means "no precision"; a negative precision
	 * from a '.*' argument is normalized to -1. */
	info->prec = p < 0 ? -1 : p;
	info->width = w;
	info->left = (fl & LEFT_ADJ) != 0;
	info->showsign = (fl & MARK_POS) != 0;
	info->space = (fl & PAD_POS) != 0;
	info->alt = (fl & ALT_FORM) != 0;
	info->group = (fl & GROUPED) != 0;
	/* Zero-padding is cleared when the '-' flag is present. */
	info->pad = ((fl & ZERO_PAD) && !(fl & LEFT_ADJ)) ? '0' : ' ';
	info->spec = (wchar_t)ch;
	info->user = user_mod;
	info->wide = 1;
	/* Map the length-modifier state to the printf_info flags.
	 * printf_info has no dedicated field for "ll" and treats
	 * a double 'l' like 'L'. */
	switch (ps) {
	case LPRE:
		info->is_long = 1;
		break;
	case LLPRE:
		info->is_long_double = 1;
		break;
	case BIGLPRE:
		info->is_long_double = 1;
		break;
	case HPRE:
		info->is_short = 1;
		break;
	case HHPRE:
		info->is_char = 1;
		break;
	case ZTPRE:
	case JPRE:
		info->is_long = 1;
		break;
	default:
		break;
	}
}

/* Validate one arginfo type. Fills the internal pop_arg type for
 * basic types (ast), leaves -1 for user-defined types. Returns 0 if
 * the type is usable, -1 otherwise. */
static int check_one_argtype(int at, int *ast)
{
	if (at & PA_FLAG_MASK) {
		/* Basic type with length flags. */
		*ast = pa_to_st(at);
	} else if (at < PA_LAST) {
		*ast = pa_to_st(at);
	} else {
		*ast = -1;
		int ok = at < PA_TYPE_MAX &&
		         __printf_va_arg_table != NULL &&
		         __printf_va_arg_table[at - PA_LAST] != NULL;
		if (!ok) {
			return -1;
		}
		return 0;
	}
	if (*ast < 0) {
		return -1;
	}
	return 0;
}

/* Query arginfo_fn and validate every requested type before any
 * argument is consumed. Returns the argument count, or -1 if the
 * arginfo callback misbehaves. */
static int validate_arg_types(printf_arginfo_size_function *arginfo_fn,
                              const struct printf_info *info,
                              int *atypes, int *asizes, int *ast)
{
	int ndata = arginfo_fn ? arginfo_fn(info, 0, 0, 0) : 0;
	if (ndata < 0 || ndata > 32) {
		return -1;
	}
	if (ndata > 0) {
		ndata = arginfo_fn(info, (size_t)ndata, atypes, asizes);
		if (ndata < 0 || ndata > 32) {
			return -1;
		}
	}

	/* Validate all types before consuming any argument, so a bogus
	 * arginfo cannot leave the va_list half way through on error. */
	for (int j = 0; j < ndata; j++) {
		if (check_one_argtype(atypes[j], &ast[j]) != 0) {
			return -1;
		}
	}
	return ndata;
}

/* Collect the arguments described by atypes/asizes/ast from the
 * va_list. Returns 0 on success, -1 if memory allocation failed
 * (already-collected user buffers are freed). */
static int collect_reg_args(va_list *ap, const int *atypes, const int *asizes,
                            const int *ast, int ndata,
                            void **args_ptr, union arg *a, void **user_mem)
{
	for (int j = 0; j < ndata; j++) {
		user_mem[j] = 0;
		/* Basic types carrying length flags must go through pop_arg;
		 * only flag-free ids at/above PA_LAST are user-registered
		 * types (kept consistent with check_one_argtype). */
		if ((atypes[j] & PA_FLAG_MASK) == 0 && atypes[j] >= PA_LAST) {
			user_mem[j] = malloc(asizes[j] ? asizes[j] : sizeof(void *));
			if (!user_mem[j]) {
				for (int k = 0; k < j; k++) {
					if (user_mem[k]) {
						free(user_mem[k]);
					}
				}
				return -1;
			}
			__printf_va_arg_table[atypes[j] - PA_LAST](user_mem[j], ap);
			/* ABI contract: args[j] points to a slot holding a
			 * POINTER to the user data, so handlers dereference
			 * one extra level. */
			a[j].p = user_mem[j];
			args_ptr[j] = &a[j];
		} else {
			if (ast[j] == DBL) {
				/* The handler ABI expects raw double bits at
				 * offset 0; the internal union stores doubles
				 * as long double, so store the raw bits here. */
				double dv = va_arg(*ap, double);
				__builtin_memcpy(&a[j], &dv, sizeof(dv));
			} else {
				pop_arg(&a[j], ast[j], ap);
			}
			args_ptr[j] = &a[j];
		}
	}
	return 0;
}

/* Handle a registered (user-defined) conversion specifier on a wide
 * stream. Must be called with *s pointing at the specifier character,
 * in the real (non-pre-pass) round.
 * Returns the number of characters written, -1 for an invalid format,
 * -2 if memory allocation failed, -3 if the handler reported an
 * error (errno preserved), or -4 if the handler declined the
 * conversion (falling back to the builtin, which for a
 * non-builtin character is an invalid format). */
static int wprintf_registered(FILE *f, const wchar_t **s, va_list *ap,
                               unsigned fl, int w, int p,
                               unsigned int user_mod, unsigned ps)
{
	printf_function *handler;
	struct printf_info info;
	int atypes[32];
	int asizes[32];
	int ast[32];
	void *args_ptr[32];
	union arg a[32];
	void *user_mem[32];
	int ndata;
	int ret;
	unsigned char ch = (unsigned char)**s;

	handler = __printf_function_table[ch];
	fill_printf_info(&info, ch, fl, w, p, user_mod, ps);

	ndata = validate_arg_types(__printf_arginfo_table[ch], &info,
	                           atypes, asizes, ast);
	if (ndata < 0) {
		return -1;
	}

	if (collect_reg_args(ap, atypes, asizes, ast, ndata,
	                     args_ptr, a, user_mem) != 0) {
		return -2;
	}

	(*s)++;
	ret = handler(f, &info, (const void *const *)args_ptr);

	for (int j = 0; j < ndata; j++) {
		if (user_mem[j]) {
			free(user_mem[j]);
		}
	}
	if (ret == -2) {
		/* The handler declined this conversion; skip it and
		 * continue with the rest of the format. */
		return -4;
	}
	if (ret < 0) {
		/* Handler error; preserve errno for the caller. */
		return -3;
	}
	return ret;
}
#endif

static void out(FILE *f, const wchar_t *s, size_t l)
{
	while (l-- && !ferror(f)) fputwc(*s++, f);
}

static void pad(FILE *f, int n, int fl)
{
	if ((fl & LEFT_ADJ) || !n || ferror(f)) return;
	fprintf(f, "%*s", n, "");
}

static int getint(wchar_t **s) {
	int i;
	for (i=0; iswdigit(**s); (*s)++) {
		if (i > INT_MAX/10U || **s-'0' > INT_MAX-10*i) i = -1;
		else i = 10*i + (**s-'0');
	}
	return i;
}

static const char sizeprefix['y'-'a'] = {
['a'-'a']='L', ['e'-'a']='L', ['f'-'a']='L', ['g'-'a']='L',
['d'-'a']='j', ['i'-'a']='j', ['o'-'a']='j', ['u'-'a']='j', ['x'-'a']='j',
['p'-'a']='j'
};

static int wprintf_core(FILE *f, const wchar_t *fmt, va_list *ap, union arg *nl_arg, int *nl_type)
{
	wchar_t *a, *z, *s=(wchar_t *)fmt;
	unsigned l10n=0, fl;
	int w, p, xp;
	union arg arg;
	int argpos;
	unsigned st, ps;
	int cnt=0, l=0;
	int i;
	int t;
	char *bs;
	char charfmt[16];
	wchar_t wc;
#ifdef MUSL_EXTERNAL_FUNCTION
	int saw_reg = 0;
#endif

	for (;;) {
		/* This error is only specified for snprintf, but since it's
		 * unspecified for other forms, do the same. Stop immediately
		 * on overflow; otherwise %n could produce wrong results. */
		if (l > INT_MAX - cnt) goto overflow;

		/* Update output count, end loop when fmt is exhausted */
		cnt += l;
		if (!*s) break;

		/* Handle literal text and %% format specifiers */
		for (a=s; *s && *s!='%'; s++);
		for (z=s; s[0]=='%' && s[1]=='%'; z++, s+=2);
		if (z-a > INT_MAX-cnt) goto overflow;
		l = z-a;
		if (f) out(f, a, l);
		if (l) continue;

		if (iswdigit(s[1]) && s[2]=='$') {
			l10n=1;
			argpos = s[1]-'0';
			s+=3;
		} else {
			argpos = -1;
			s++;
		}

		/* Read modifier flags */
		for (fl=0; (unsigned)*s-' '<32 && (FLAGMASK&(1U<<*s-' ')); s++)
			fl |= 1U<<*s-' ';

		/* Read field width */
		if (*s=='*') {
			if (iswdigit(s[1]) && s[2]=='$') {
				l10n=1;
				nl_type[s[1]-'0'] = INT;
				w = nl_arg[s[1]-'0'].i;
				s+=3;
			} else if (!l10n) {
				w = f ? va_arg(*ap, int) : 0;
				s++;
			} else goto inval;
			if (w<0) fl|=LEFT_ADJ, w=-w;
		} else if ((w=getint(&s))<0) goto overflow;

		/* Read precision */
		if (*s=='.' && s[1]=='*') {
			if (isdigit(s[2]) && s[3]=='$') {
				nl_type[s[2]-'0'] = INT;
				p = nl_arg[s[2]-'0'].i;
				s+=4;
			} else if (!l10n) {
				p = f ? va_arg(*ap, int) : 0;
				s+=2;
			} else goto inval;
			xp = (p>=0);
		} else if (*s=='.') {
			s++;
			p = getint(&s);
			xp = 1;
		} else {
			p = -1;
			xp = 0;
		}

#ifdef MUSL_EXTERNAL_FUNCTION
		unsigned int user_mod = 0;
		if (__printf_modifier_table) {
			/* Match a registered wide modifier and record its bit;
			 * parsing then continues with the conversion specifier. */
			__handle_registered_modifier_wc((const wchar_t **)&s, &user_mod);
		}
#endif

		/* Format specifier state machine */
		st=0;
		do {
			if (OOB(*s)) goto inval;
			ps=st;
			st=states[st]S(*s++);
		} while (st-1<STOP);
		if (!st) {
#ifdef MUSL_EXTERNAL_FUNCTION
			if (!f && __printf_function_table) {
				s--;
				unsigned char ch = (unsigned char)*s;
				if (ch <= UCHAR_MAX && __printf_function_table[ch]) {
					/* Pre-pass: a registered specifier contributes
					 * no nl_type entry. Skip it and keep scanning;
					 * mixing positional arguments with registered
					 * specifiers is rejected below. */
					saw_reg = 1;
					s++;
					continue;
				}
			}
			if (f && __printf_function_table) {
				s--;
				unsigned char ch = (unsigned char)*s;
				if (ch <= UCHAR_MAX && __printf_function_table[ch]) {
					int ret = wprintf_registered(f, &s, ap, fl, w, p,
					                             user_mod, ps);
					if (ret >= 0) { l = ret; continue; }
					if (ret == -1 || ret == -4) goto inval;
					if (ret == -2) goto user_ovf;
					return -1;
				}
			}
#endif
			goto inval;
		}

		/* Check validity of argument type (nl/normal) */
		if (st==NOARG) {
			if (argpos>=0) goto inval;
		} else {
			if (argpos>=0) nl_type[argpos]=st, arg=nl_arg[argpos];
			else if (f) pop_arg(&arg, st, ap);
			else return 0;
		}

		if (!f) continue;

		/* Do not process any new directives once in error state. */
		if (ferror(f)) return -1;

		t = s[-1];
		if (ps && (t&15)==3) t&=~32;

		switch (t) {
		case 'n':
			switch(ps) {
			case BARE: *(int *)arg.p = cnt; break;
			case LPRE: *(long *)arg.p = cnt; break;
			case LLPRE: *(long long *)arg.p = cnt; break;
			case HPRE: *(unsigned short *)arg.p = cnt; break;
			case HHPRE: *(unsigned char *)arg.p = cnt; break;
			case ZTPRE: *(size_t *)arg.p = cnt; break;
			case JPRE: *(uintmax_t *)arg.p = cnt; break;
			}
			continue;
		case 'c':
		case 'C':
			if (w<1) w=1;
			pad(f, w-1, fl);
			out(f, &(wchar_t){t=='C' ? arg.i : btowc(arg.i)}, 1);
			pad(f, w-1, fl^LEFT_ADJ);
			l = w;
			continue;
		case 'S':
			a = arg.p;
			z = a + wcsnlen(a, p<0 ? INT_MAX : p);
			if (p<0 && *z) goto overflow;
			p = z-a;
			if (w<p) w=p;
			pad(f, w-p, fl);
			out(f, a, p);
			pad(f, w-p, fl^LEFT_ADJ);
			l=w;
			continue;
		case 'm':
			arg.p = strerror(errno);
		case 's':
			if (!arg.p) arg.p = "(null)";
			bs = arg.p;
			for (i=l=0; l<(p<0?INT_MAX:p) && (i=mbtowc(&wc, bs, MB_LEN_MAX))>0; bs+=i, l++);
			if (i<0) return -1;
			if (p<0 && *bs) goto overflow;
			p=l;
			if (w<p) w=p;
			pad(f, w-p, fl);
			bs = arg.p;
			while (l--) {
				i=mbtowc(&wc, bs, MB_LEN_MAX);
				bs+=i;
				out(f, &wc, 1);
			}
			pad(f, w-p, fl^LEFT_ADJ);
			l=w;
			continue;
		}

		if (xp && p<0) goto overflow;
		snprintf(charfmt, sizeof charfmt, "%%%s%s%s%s%s*.*%c%c",
			"#"+!(fl & ALT_FORM),
			"+"+!(fl & MARK_POS),
			"-"+!(fl & LEFT_ADJ),
			" "+!(fl & PAD_POS),
			"0"+!(fl & ZERO_PAD),
			sizeprefix[(t|32)-'a'], t);

		switch (t|32) {
		case 'a': case 'e': case 'f': case 'g':
			l = fprintf(f, charfmt, w, p, arg.f);
			break;
		case 'd': case 'i': case 'o': case 'u': case 'x': case 'p':
			l = fprintf(f, charfmt, w, p, arg.i);
			break;
		}
	}

	if (f) return cnt;
#ifdef MUSL_EXTERNAL_FUNCTION
	/* Mixing positional arguments with registered specifiers is not
	 * supported: the pre-pass cannot type the registered arguments. */
	if (saw_reg && l10n) goto inval;
#endif
	if (!l10n) return 0;

	for (i=1; i<=NL_ARGMAX && nl_type[i]; i++)
		pop_arg(nl_arg+i, nl_type[i], ap);
	for (; i<=NL_ARGMAX && !nl_type[i]; i++);
	if (i<=NL_ARGMAX) return -1;
	return 1;

inval:
	errno = EINVAL;
	return -1;
overflow:
	errno = EOVERFLOW;
	return -1;
#ifdef MUSL_EXTERNAL_FUNCTION
user_ovf:
	errno = ENOMEM;
	return -1;
#endif
}

int vfwprintf(FILE *restrict f, const wchar_t *restrict fmt, va_list ap)
{
	va_list ap2;
	int nl_type[NL_ARGMAX+1] = {0};
	union arg nl_arg[NL_ARGMAX+1];
	int olderr;
	int ret;

	/* the copy allows passing va_list* even if va_list is an array */
	va_copy(ap2, ap);
	if (wprintf_core(0, fmt, &ap2, nl_arg, nl_type) < 0) {
		va_end(ap2);
		return -1;
	}

	FLOCK(f);
	fwide(f, 1);
	olderr = f->flags & F_ERR;
	f->flags &= ~F_ERR;
	ret = wprintf_core(f, fmt, &ap2, nl_arg, nl_type);
	if (ferror(f)) ret = -1;
	f->flags |= olderr;
	FUNLOCK(f);
	va_end(ap2);
	return ret;
}
