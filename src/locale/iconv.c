#include <iconv.h>
#include <errno.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "locale_impl.h"
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
#include <info/device_api_version.h>
#endif
#endif

#define UTF_32BE    0300
#define UTF_16LE    0301
#define UTF_16BE    0302
#define UTF_32LE    0303
#define UCS2BE      0304
#define UCS2LE      0305
#define WCHAR_T     0306
#define US_ASCII    0307
#define UTF_8       0310
#define UTF_16      0312
#define UTF_32      0313
#define UCS2        0314
#define EUC_JP      0320
#define SHIFT_JIS   0321
#define ISO2022_JP  0322
#define GB18030     0330
#define GBK         0331
#define GB2312      0332
#define BIG5        0340
#define EUC_KR      0350
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
#define ICU_ZERO_ERROR 0
#define ICU_IVALID_CHAR_ERROR 10
#define ICU_TRUNCATED_CHAR_ERROR 11
#define ICU_ILLEGAL_CHAR_ERROR 12
#define ICU_BUFFER_OVERFLOW_ERROR 15
#define ICU_SKIP_THRESHOLD 2
#define DEVICE_VERSION_THRESHOLD 16
#define TYPE_FLAG_POS 1
#define TO_IGNORE_FLAG_POS 2
#define FROM_IGNORE_FLAG_POS 3
#define TO_TRANSLIT_FLAG_POS 4
#define FROM_TRANSLIT_FLAG_POS 5
#endif
#endif
/* Definitions of charmaps. Each charmap consists of:
 * 1. Empty-string-terminated list of null-terminated aliases.
 * 2. Special type code or number of elided quads of entries.
 * 3. Character table (size determined by field 2), consisting
 *    of 5 bytes for every 4 characters, interpreted as 10-bit
 *    indices into the legacy_chars table. */

static const unsigned char charmaps[] =
"utf8\0char\0\0\310"
"wchart\0\0\306"
"ucs2be\0\0\304"
"ucs2le\0\0\305"
"utf16be\0\0\302"
"utf16le\0\0\301"
"ucs4be\0utf32be\0\0\300"
"ucs4le\0utf32le\0\0\303"
"ascii\0usascii\0iso646\0iso646us\0\0\307"
"utf16\0\0\312"
"ucs4\0utf32\0\0\313"
"ucs2\0\0\314"
"eucjp\0\0\320"
"shiftjis\0sjis\0cp932\0\0\321"
"iso2022jp\0\0\322"
"gb18030\0\0\330"
"gbk\0\0\331"
"gb2312\0\0\332"
"big5\0bigfive\0cp950\0big5hkscs\0\0\340"
"euckr\0ksc5601\0ksx1001\0cp949\0\0\350"
#include "codepages.h"
;

#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
// \0 split alias;  \0\0 split name in icu
static const unsigned char icu_name_maps[] =
"utf8\0char\0\0UTF-8\0"
"utf7\0\0UTF-7\0"
"ucs2\0utf16\0ucs2be\0utf16be\0\0UTF-16BE\0"
"ucs2le\0utf16le\0\0UTF-16LE\0"
"ucs4\0utf32\0ucs4be\0utf32be\0\0UTF-32BE\0"
"wchart\0ucs4le\0utf32le\0\0UTF-32LE\0"
"ascii\0usascii\0""20127\0iso646\0iso646us\0\0US-ASCII\0"
"eucjp\0eucjp2007\0\0euc-jp-2007\0"
"shiftjis\0sjis\0cp932\0ibm943p15a2003\0\0ibm-943_P15A-2003\0"
"gb18030\0\0gb18030\0"
"gbk\0""54936\0windows9362000\0\0windows-936-2000\0"
"gb2312\0""52936\0ibm1383p1101999\0\0ibm-1383_P110-1999\0"
"big5\0""950\0bigfive\0cp950\0windows9502000\0\0windows-950-2000\0"
"big5hk\0big5hkscs\0""951\0ibm1375p1002008\0\0ibm-1375_P100-2008\0"
"euckr\0ibm970p110p1102006u2\0\0ibm-970_P110_P110-2006_U2\0"
"ksc5601\0ksx1001\0cp949\0windows9492000\0\0windows-949-2000\0"
"iso88591\0latin1\0\0ISO-8859-1\0"
"iso88592\0ibm912p1001995\0\0ibm-912_P100-1995\0"
"iso88593\0ibm913p1002000\0\0ibm-913_P100-2000\0"
"iso88594\0ibm914p1001995\0\0ibm-914_P100-1995\0"
"iso88595\0ibm915p1001995\0\0ibm-915_P100-1995\0"
"iso88596\0ibm1089p1001995\0\0ibm-1089_P100-1995\0"
"iso88597\0ibm9005x1102007\0\0ibm-9005_X110-2007\0"
"iso88598\0ibm5012p1001999\0\0ibm-5012_P100-1999\0"
"iso88599\0ibm920p1001995\0\0ibm-920_P100-1995\0"
"iso885910\0iso8859101998\0\0iso-8859_10-1998\0"
"iso885911\0iso8859112001\0\0iso-8859_11-2001\0"
"tis620\0windows8742000\0\0windows-874-2000\0"
"iso885913\0ibm921p1001995\0\0ibm-921_P100-1995\0"
"iso885914\0iso8859141998\0\0iso-8859_14-1998\0"
"iso885915\0latin9\0ibm923p1001998\0\0ibm-923_P100-1998\0"
"cp1250\0windows1250\0ibm5346p1001998\0\0ibm-5346_P100-1998\0"
"cp1251\0windows1251\0ibm5347p1001998\0\0ibm-5347_P100-1998\0"
"cp1252\0windows1252\0ibm5348p1001997\0\0ibm-5348_P100-1997\0"
"cp1253\0windows1253\0ibm5349p1001998\0\0ibm-5349_P100-1998\0"
"cp1254\0windows1254\0ibm5350p1001998\0\0ibm-5350_P100-1998\0"
"cp1255\0windows1255\0ibm9447p1002002\0\0ibm-9447_P100-2002\0"
"cp1256\0windows1256\0ibm9448x1002005\0\0ibm-9448_X100-2005\0"
"cp1257\0windows1257\0ibm9449p1002002\0\0ibm-9449_P100-2002\0"
"cp1258\0windows1258\0ibm5354p1001998\0\0ibm-5354_P100-1998\0"
"koi8r\0ibm878p1001996\0\0ibm-878_P100-1996\0"
"koi8u\0ibm1168p1002002\0\0ibm-1168_P100-2002\0"
"cp437\0ibm437p1001995\0\0ibm-437_P100-1995\0"
"cp850\0ibm850p1001995\0\0ibm-850_P100-1995\0"
"cp866\0ibm866p1001995\0\0ibm-866_P100-1995\0"
"ibm1047\0cp1047\0ibm1047p1001995\0\0ibm-1047_P100-1995\0"
;
#endif
#endif

/* Table of characters that appear in legacy 8-bit codepages,
 * limited to 1024 slots (10 bit indices). The first 256 entries
 * are elided since those characters are obviously all included. */
static const unsigned short legacy_chars[] = {
#include "legacychars.h"
};

static const unsigned short jis0208[84][94] = {
#include "jis0208.h"
};

static const unsigned short gb18030[126][190] = {
#include "gb18030.h"
};

static const unsigned short big5[89][157] = {
#include "big5.h"
};

static const unsigned short hkscs[] = {
#include "hkscs.h"
};

static const unsigned short ksc[93][94] = {
#include "ksc.h"
};

static const unsigned short rev_jis[] = {
#include "revjis.h"
};

static int fuzzycmp(const unsigned char *a, const unsigned char *b)
{
	for (; *a && *b; a++, b++) {
		while (*a && (*a|32U)-'a'>26 && *a-'0'>10U) a++;
		if ((*a|32U) != *b) return 1;
	}
	return *a != *b;
}

static size_t find_charmap(const void *name)
{
	const unsigned char *s;
	if (!*(char *)name) name=charmaps; /* "utf8" */
	for (s=charmaps; *s; ) {
		if (!fuzzycmp(name, s)) {
			for (; *s; s+=strlen((void *)s)+1);
			return s+1-charmaps;
		}
		s += strlen((void *)s)+1;
		if (!*s) {
			if (s[1] > 0200) s+=2;
			else s+=2+(64U-s[1])*5;
		}
	}
	return -1;
}

#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
static const unsigned char* find_icu_map(const void *query_name)
{
    if (!*(char *)query_name) {
        query_name = icu_name_maps;
    }

    const unsigned char *icu_name = icu_name_maps;
    while (*icu_name) {
        if (!fuzzycmp(query_name, icu_name)) {
            while (*icu_name) {
                icu_name += strlen((void *)icu_name) + 1;  //find nearly \0\0
            }
            return icu_name + 1;
        }
        icu_name += strlen((void *)icu_name) + 1;  // skip \0
        if (!*icu_name) {  // skip \0\0
            icu_name++;
            while (*icu_name) {icu_name++;}
            icu_name++;
        }
    }
    return NULL;
}
#endif
#endif

struct stateful_cd {
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
    unsigned sign;
    const unsigned char* to;
    const unsigned char* from;
#endif
#endif
	iconv_t base_cd;
	unsigned state;
};

static iconv_t combine_to_from(size_t t, size_t f)
{
	return (void *)(f<<16 | t<<1 | 1);
}

static size_t extract_from(iconv_t cd)
{
	return (size_t)cd >> 16;
}

static size_t extract_to(iconv_t cd)
{
	return (size_t)cd >> 1 & 0x7fff;
}

#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
static void set_type_flag(unsigned* value) {*value = (1 << TYPE_FLAG_POS) | *value;}
static void set_to_ignore_flag(unsigned* value) {*value = (1 << TO_IGNORE_FLAG_POS) | *value;}
static void set_from_ignore_flag(unsigned* value) {*value = (1 << FROM_IGNORE_FLAG_POS) | *value;}
static void set_to_translit_flag(unsigned* value) {*value = (1 << TO_TRANSLIT_FLAG_POS) | *value;}
static void set_from_translit_flag(unsigned* value) {*value = (1 << FROM_TRANSLIT_FLAG_POS) | *value;}
static bool get_type_flag(unsigned value) {return (value >> TYPE_FLAG_POS) & 1;}
static bool get_to_ignore_flag(unsigned value) {return (value >> TO_IGNORE_FLAG_POS) & 1;}
static bool get_from_ignore_flag(unsigned value) {return (value >> FROM_IGNORE_FLAG_POS) & 1;}
static bool get_to_translit_flag(unsigned value) {return (value >> TO_TRANSLIT_FLAG_POS) & 1;}
static bool get_from_translit_flag(unsigned value) {return (value >> FROM_TRANSLIT_FLAG_POS) & 1;}

static bool deal_with_tail(const char* ins, unsigned* sign, const unsigned char** res, bool is_from)
{
    char* ins_tmp = strdup(ins);
    if (!ins_tmp) {return false;}
    char* ins_ignore_pos = strstr(ins_tmp, "//IGNORE");
    char* ins_translit_pos = strstr(ins_tmp, "//TRANSLIT");
    if (ins_ignore_pos) {
        if (is_from) {
            set_from_ignore_flag(sign);
        } else {
            set_to_ignore_flag(sign);
        }
        *ins_ignore_pos = '\0';
        *res = find_icu_map((void*)ins_tmp);
    } else if (ins_translit_pos) {
        if (is_from) {
            set_from_translit_flag(sign);
        } else {
            set_to_translit_flag(sign);
        }
        *ins_translit_pos = '\0';
        *res = find_icu_map((void*)ins_tmp);
    } else {
        *res = find_icu_map(ins);
    }
    free(ins_tmp);
    return true;
}
#endif
#endif

iconv_t iconv_open(const char *to, const char *from)
{
    struct stateful_cd *scd;

#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
    bool is_basic_open = false;
    if (get_device_api_version_inner() < DEVICE_VERSION_THRESHOLD) {
        is_basic_open = true;
    } else {
        for (const char* s = "iso885916\0iso2022jp\0\0"; *s;) {  // icu not support
            if (!fuzzycmp((void*)to, (void*)s) || !fuzzycmp((void*)from, (void*)s)) {
                is_basic_open = true;
            }
            s += strlen(s) + 1;
        }
    }

    // icu open
    if (!is_basic_open && icuuc_handle_init()) {
        scd = malloc(sizeof *scd);
        if (!scd) {return (iconv_t)-1;}
        scd->sign = 0;
        scd->state = 0;

        if (!deal_with_tail(to, &scd->sign, &scd->to, false)) {return (iconv_t)-1;}
        if (!deal_with_tail(from, &scd->sign, &scd->from, true)) {return (iconv_t)-1;}

        if (!scd->to || !scd->from) {
            errno = EINVAL;
            free(scd);
            return (iconv_t)-1;
        }

        set_type_flag(&scd->sign);
        return (iconv_t)scd;
    }
#endif
#endif

    // basic open
    size_t f, t;
	if ((t = find_charmap(to))==-1
	 || (f = find_charmap(from))==-1
	 || (charmaps[t] >= 0330)) {
		errno = EINVAL;
		return (iconv_t)-1;
	}
	iconv_t cd = combine_to_from(t, f);

	switch (charmaps[f]) {
	case UTF_16:
	case UTF_32:
	case UCS2:
	case ISO2022_JP:
		scd = malloc(sizeof *scd);
		if (!scd) return (iconv_t)-1;
        memset(scd, 0, sizeof(*scd));
		scd->base_cd = cd;
		scd->state = 0;
		cd = (iconv_t)scd;
	}

	return cd;
}

static unsigned get_16(const unsigned char *s, int e)
{
	e &= 1;
	return s[e]<<8 | s[1-e];
}

static void put_16(unsigned char *s, unsigned c, int e)
{
	e &= 1;
	s[e] = c>>8;
	s[1-e] = c;
}

static unsigned get_32(const unsigned char *s, int e)
{
	e &= 3;
	return s[e]+0U<<24 | s[e^1]<<16 | s[e^2]<<8 | s[e^3];
}

static void put_32(unsigned char *s, unsigned c, int e)
{
	e &= 3;
	s[e^0] = c>>24;
	s[e^1] = c>>16;
	s[e^2] = c>>8;
	s[e^3] = c;
}

/* Adapt as needed */
#define mbrtowc_utf8 mbrtowc
#define wctomb_utf8 wctomb

static unsigned legacy_map(const unsigned char *map, unsigned c)
{
	if (c < 4*map[-1]) return c;
	unsigned x = c - 4*map[-1];
	x = map[x*5/4]>>2*x%8 | map[x*5/4+1]<<8-2*x%8 & 1023;
	return x < 256 ? x : legacy_chars[x-256];
}

static unsigned uni_to_jis(unsigned c)
{
	unsigned nel = sizeof rev_jis / sizeof *rev_jis;
	unsigned d, j, i, b = 0;
	for (;;) {
		i = nel/2;
		j = rev_jis[b+i];
		d = jis0208[j/256][j%256];
		if (d==c) return j + 0x2121;
		else if (nel == 1) return 0;
		else if (c < d)
			nel /= 2;
		else {
			b += i;
			nel -= nel/2;
		}
	}
}

#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
static void ucnv_from_u_callback_ignore(
    const void* context,
    void* fromUArgs,
    const void* codeUnits,
    int32_t length,
    int32_t codePoint,
    int reason,
    int* err)
{
    if (reason <= ICU_SKIP_THRESHOLD) {
        *err = ICU_ZERO_ERROR;
    }
}

static void ucnv_from_u_callback_stop(const void* context, ...) { }

static void ucnv_to_u_callback_ignore(
    const void* context,
    void* toUArgs,
    const void* codeUnits,
    int32_t length,
    int reason,
    int* err)
{
    if (reason <= ICU_SKIP_THRESHOLD) {
        *err = ICU_ZERO_ERROR;
    }
}

static void ucnv_to_u_callback_stop(const void* context, ...) { }

static void set_errno(int errCode)
{
    if (errCode == ICU_ZERO_ERROR) {
        errno = 0;
    } else if (errCode == ICU_BUFFER_OVERFLOW_ERROR) {
        errno = E2BIG;
    } else if (errCode == ICU_IVALID_CHAR_ERROR ||
               errCode == ICU_TRUNCATED_CHAR_ERROR ||
               errCode == ICU_ILLEGAL_CHAR_ERROR) {
        errno = EILSEQ;
    } else {
        errno = EINVAL;
    }
}

static size_t iconv_icu(unsigned sign, const unsigned char* to, const unsigned char* from,
char **restrict in, size_t *restrict inb, char **restrict out, size_t *restrict outb)
{
    int errCode = ICU_ZERO_ERROR;
    size_t out_size = 0;
    size_t uchars_len = *inb * 4;
    uint16_t uchars[uchars_len];

    // (from -> UChars) <=> ucnv_toUChars
    void* conv_to_u = g_icu_opt_func.ucnv_open((void*)from, &errCode);
    if (get_from_ignore_flag(sign)) {
        g_icu_opt_func.ucnv_setToUCallBack(conv_to_u, ucnv_to_u_callback_ignore, NULL, NULL, NULL, &errCode);
    } else if (!get_from_translit_flag(sign)) {
        g_icu_opt_func.ucnv_setFromUCallBack(conv_to_u, ucnv_to_u_callback_stop, NULL, NULL, NULL, &errCode);
    }
    uchars_len = g_icu_opt_func.ucnv_toUChars(conv_to_u, uchars, uchars_len, *in, *inb, &errCode);
    if (errCode > ICU_ZERO_ERROR) {
        set_errno(errCode);
        return (size_t)-1;
    } else {
        errCode = ICU_ZERO_ERROR;
    }
    g_icu_opt_func.ucnv_close(conv_to_u);

    // (UChars -> to) <=> ucnv_fromUChars
    void* conv_from_u = g_icu_opt_func.ucnv_open((void*)to, &errCode);
    if (get_to_ignore_flag(sign)) {
        g_icu_opt_func.ucnv_setFromUCallBack(conv_from_u, ucnv_from_u_callback_ignore, NULL, NULL, NULL, &errCode);
    } else if (!get_to_translit_flag(sign)) {
        g_icu_opt_func.ucnv_setFromUCallBack(conv_from_u, ucnv_from_u_callback_stop, NULL, NULL, NULL, &errCode);
    }
    out_size = g_icu_opt_func.ucnv_fromUChars(conv_from_u, *out, *outb, uchars, uchars_len, &errCode);
    if (errCode > ICU_ZERO_ERROR) {
        set_errno(errCode);
        return (size_t)-1;
    } else {
        errCode = ICU_ZERO_ERROR;
    }
    g_icu_opt_func.ucnv_close(conv_from_u);

    *out += out_size;
    *outb -= out_size;
    *in += *inb;
    *inb -= *inb;
    set_errno(errCode);

    return (size_t)errCode;
}
#endif
#endif

size_t iconv(iconv_t cd, char **restrict in, size_t *restrict inb, char **restrict out, size_t *restrict outb)
{
    if (!in || !*in || !*inb) {
        return 0;
    }

    size_t x=0;
	struct stateful_cd *scd=0;
	if (!((size_t)cd & 1)) {
		scd = (void *)cd;
		cd = scd->base_cd;
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
        if (get_type_flag(scd->sign)) {
            return iconv_icu(scd->sign, scd->to, scd->from, in, inb, out, outb);
        }
#endif
#endif
	}
	unsigned to = extract_to(cd);
	unsigned from = extract_from(cd);
	const unsigned char *map = charmaps+from+1;
	const unsigned char *tomap = charmaps+to+1;
	mbstate_t st = {0};
	wchar_t wc;
	unsigned c, d;
	size_t k, l;
	int err;
	unsigned char type = map[-1];
	unsigned char totype = tomap[-1];
	locale_t *ploc = &CURRENT_LOCALE, loc = *ploc;

	*ploc = UTF8_LOCALE;

	for (; *inb; *in+=l, *inb-=l) {
		c = *(unsigned char *)*in;
		l = 1;

		switch (type) {
		case UTF_8:
			if (c < 128) break;
			l = mbrtowc_utf8(&wc, *in, *inb, &st);
			if (l == (size_t)-1) goto ilseq;
			if (l == (size_t)-2) goto starved;
			c = wc;
			break;
		case US_ASCII:
			if (c >= 128) goto ilseq;
			break;
		case WCHAR_T:
			l = sizeof(wchar_t);
			if (*inb < l) goto starved;
			c = *(wchar_t *)*in;
			if (0) {
		case UTF_32BE:
		case UTF_32LE:
			l = 4;
			if (*inb < 4) goto starved;
			c = get_32((void *)*in, type);
			}
			if (c-0xd800u < 0x800u || c >= 0x110000u) goto ilseq;
			break;
		case UCS2BE:
		case UCS2LE:
		case UTF_16BE:
		case UTF_16LE:
			l = 2;
			if (*inb < 2) goto starved;
			c = get_16((void *)*in, type);
			if ((unsigned)(c-0xdc00) < 0x400) goto ilseq;
			if ((unsigned)(c-0xd800) < 0x400) {
				if (type-UCS2BE < 2U) goto ilseq;
				l = 4;
				if (*inb < 4) goto starved;
				d = get_16((void *)(*in + 2), type);
				if ((unsigned)(d-0xdc00) >= 0x400) goto ilseq;
				c = ((c-0xd7c0)<<10) + (d-0xdc00);
			}
			break;
		case UCS2:
		case UTF_16:
			l = 0;
			if (!scd->state) {
				if (*inb < 2) goto starved;
				c = get_16((void *)*in, 0);
				scd->state = type==UCS2
					? c==0xfffe ? UCS2LE : UCS2BE
					: c==0xfffe ? UTF_16LE : UTF_16BE;
				if (c == 0xfffe || c == 0xfeff)
					l = 2;
			}
			type = scd->state;
			continue;
		case UTF_32:
			l = 0;
			if (!scd->state) {
				if (*inb < 4) goto starved;
				c = get_32((void *)*in, 0);
				scd->state = c==0xfffe0000 ? UTF_32LE : UTF_32BE;
				if (c == 0xfffe0000 || c == 0xfeff)
					l = 4;
			}
			type = scd->state;
			continue;
		case SHIFT_JIS:
			if (c < 128) break;
			if (c-0xa1 <= 0xdf-0xa1) {
				c += 0xff61-0xa1;
				break;
			}
			l = 2;
			if (*inb < 2) goto starved;
			d = *((unsigned char *)*in + 1);
			if (c-129 <= 159-129) c -= 129;
			else if (c-224 <= 239-224) c -= 193;
			else goto ilseq;
			c *= 2;
			if (d-64 <= 158-64) {
				if (d==127) goto ilseq;
				if (d>127) d--;
				d -= 64;
			} else if (d-159 <= 252-159) {
				c++;
				d -= 159;
			}
			c = jis0208[c][d];
			if (!c) goto ilseq;
			break;
		case EUC_JP:
			if (c < 128) break;
			l = 2;
			if (*inb < 2) goto starved;
			d = *((unsigned char *)*in + 1);
			if (c==0x8e) {
				c = d;
				if (c-0xa1 > 0xdf-0xa1) goto ilseq;
				c += 0xff61 - 0xa1;
				break;
			}
			c -= 0xa1;
			d -= 0xa1;
			if (c >= 84 || d >= 94) goto ilseq;
			c = jis0208[c][d];
			if (!c) goto ilseq;
			break;
		case ISO2022_JP:
			if (c >= 128) goto ilseq;
			if (c == '\033') {
				l = 3;
				if (*inb < 3) goto starved;
				c = *((unsigned char *)*in + 1);
				d = *((unsigned char *)*in + 2);
				if (c != '(' && c != '$') goto ilseq;
				switch (128*(c=='$') + d) {
				case 'B': scd->state=0; continue;
				case 'J': scd->state=1; continue;
				case 'I': scd->state=4; continue;
				case 128+'@': scd->state=2; continue;
				case 128+'B': scd->state=3; continue;
				}
				goto ilseq;
			}
			switch (scd->state) {
			case 1:
				if (c=='\\') c = 0xa5;
				if (c=='~') c = 0x203e;
				break;
			case 2:
			case 3:
				l = 2;
				if (*inb < 2) goto starved;
				d = *((unsigned char *)*in + 1);
				c -= 0x21;
				d -= 0x21;
				if (c >= 84 || d >= 94) goto ilseq;
				c = jis0208[c][d];
				if (!c) goto ilseq;
				break;
			case 4:
				if (c-0x60 < 0x1f) goto ilseq;
				if (c-0x21 < 0x5e) c += 0xff61-0x21;
				break;
			}
			break;
		case GB2312:
			if (c < 128) break;
			if (c < 0xa1) goto ilseq;
		case GBK:
		case GB18030:
			if (c < 128) break;
			c -= 0x81;
			if (c >= 126) goto ilseq;
			l = 2;
			if (*inb < 2) goto starved;
			d = *((unsigned char *)*in + 1);
			if (d < 0xa1 && type == GB2312) goto ilseq;
			if (d-0x40>=191 || d==127) {
				if (d-'0'>9 || type != GB18030)
					goto ilseq;
				l = 4;
				if (*inb < 4) goto starved;
				c = (10*c + d-'0') * 1260;
				d = *((unsigned char *)*in + 2);
				if (d-0x81>126) goto ilseq;
				c += 10*(d-0x81);
				d = *((unsigned char *)*in + 3);
				if (d-'0'>9) goto ilseq;
				c += d-'0';
				c += 128;
				for (d=0; d<=c; ) {
					k = 0;
					for (int i=0; i<126; i++)
						for (int j=0; j<190; j++)
							if (gb18030[i][j]-d <= c-d)
								k++;
					d = c+1;
					c += k;
				}
				break;
			}
			d -= 0x40;
			if (d>63) d--;
			c = gb18030[c][d];
			break;
		case BIG5:
			if (c < 128) break;
			l = 2;
			if (*inb < 2) goto starved;
			d = *((unsigned char *)*in + 1);
			if (d-0x40>=0xff-0x40 || d-0x7f<0xa1-0x7f) goto ilseq;
			d -= 0x40;
			if (d > 0x3e) d -= 0x22;
			if (c-0xa1>=0xfa-0xa1) {
				if (c-0x87>=0xff-0x87) goto ilseq;
				if (c < 0xa1) c -= 0x87;
				else c -= 0x87 + (0xfa-0xa1);
				c = (hkscs[4867+(c*157+d)/16]>>(c*157+d)%16)%2<<17
					| hkscs[c*157+d];
				/* A few HKSCS characters map to pairs of UCS
				 * characters. These are mapped to surrogate
				 * range in the hkscs table then hard-coded
				 * here. Ugly, yes. */
				if (c/256 == 0xdc) {
					union {
						char c[8];
						wchar_t wc[2];
					} tmp;
					char *ptmp = tmp.c;
					size_t tmpx = iconv(combine_to_from(to, find_charmap("utf8")),
						&(char *){"\303\212\314\204"
						"\303\212\314\214"
						"\303\252\314\204"
						"\303\252\314\214"
						+c%256}, &(size_t){4},
						&ptmp, &(size_t){sizeof tmp});
					size_t tmplen = ptmp - tmp.c;
					if (tmplen > *outb) goto toobig;
					if (tmpx) x++;
					memcpy(*out, &tmp, tmplen);
					*out += tmplen;
					*outb -= tmplen;
					continue;
				}
				if (!c) goto ilseq;
				break;
			}
			c -= 0xa1;
			c = big5[c][d]|(c==0x27&&(d==0x3a||d==0x3c||d==0x42))<<17;
			if (!c) goto ilseq;
			break;
		case EUC_KR:
			if (c < 128) break;
			l = 2;
			if (*inb < 2) goto starved;
			d = *((unsigned char *)*in + 1);
			c -= 0xa1;
			d -= 0xa1;
			if (c >= 93 || d >= 94) {
				c += (0xa1-0x81);
				d += 0xa1;
				if (c >= 93 || c>=0xc6-0x81 && d>0x52)
					goto ilseq;
				if (d-'A'<26) d = d-'A';
				else if (d-'a'<26) d = d-'a'+26;
				else if (d-0x81<0xff-0x81) d = d-0x81+52;
				else goto ilseq;
				if (c < 0x20) c = 178*c + d;
				else c = 178*0x20 + 84*(c-0x20) + d;
				c += 0xac00;
				for (d=0xac00; d<=c; ) {
					k = 0;
					for (int i=0; i<93; i++)
						for (int j=0; j<94; j++)
							if (ksc[i][j]-d <= c-d)
								k++;
					d = c+1;
					c += k;
				}
				break;
			}
			c = ksc[c][d];
			if (!c) goto ilseq;
			break;
		default:
			if (!c) break;
			c = legacy_map(map, c);
			if (!c) goto ilseq;
		}

		switch (totype) {
		case WCHAR_T:
			if (*outb < sizeof(wchar_t)) goto toobig;
			*(wchar_t *)*out = c;
			*out += sizeof(wchar_t);
			*outb -= sizeof(wchar_t);
			break;
		case UTF_8:
			if (*outb < 4) {
				char tmp[4];
				k = wctomb_utf8(tmp, c);
				if (*outb < k) goto toobig;
				memcpy(*out, tmp, k);
			} else k = wctomb_utf8(*out, c);
			*out += k;
			*outb -= k;
			break;
		case US_ASCII:
			if (c > 0x7f) subst: x++, c='*';
		default:
			if (*outb < 1) goto toobig;
			if (c<256 && c==legacy_map(tomap, c)) {
			revout:
				if (*outb < 1) goto toobig;
				*(*out)++ = c;
				*outb -= 1;
				break;
			}
			d = c;
			for (c=4*totype; c<256; c++) {
				if (d == legacy_map(tomap, c)) {
					goto revout;
				}
			}
			goto subst;
		case SHIFT_JIS:
			if (c < 128) goto revout;
			if (c == 0xa5) {
				x++;
				c = '\\';
				goto revout;
			}
			if (c == 0x203e) {
				x++;
				c = '~';
				goto revout;
			}
			if (c-0xff61 <= 0xdf-0xa1) {
				c += 0xa1 - 0xff61;
				goto revout;
			}
			c = uni_to_jis(c);
			if (!c) goto subst;
			if (*outb < 2) goto toobig;
			d = c%256;
			c = c/256;
			*(*out)++ = (c+1)/2 + (c<95 ? 112 : 176);
			*(*out)++ = c%2 ? d + 31 + d/96 : d + 126;
			*outb -= 2;
			break;
		case EUC_JP:
			if (c < 128) goto revout;
			if (c-0xff61 <= 0xdf-0xa1) {
				c += 0x0e00 + 0x21 - 0xff61;
			} else {
				c = uni_to_jis(c);
			}
			if (!c) goto subst;
			if (*outb < 2) goto toobig;
			*(*out)++ = c/256 + 0x80;
			*(*out)++ = c%256 + 0x80;
			*outb -= 2;
			break;
		case ISO2022_JP:
			if (c < 128) goto revout;
			if (c-0xff61 <= 0xdf-0xa1 || c==0xa5 || c==0x203e) {
				if (*outb < 7) goto toobig;
				*(*out)++ = '\033';
				*(*out)++ = '(';
				if (c==0xa5) {
					*(*out)++ = 'J';
					*(*out)++ = '\\';
				} else if (c==0x203e) {
					*(*out)++ = 'J';
					*(*out)++ = '~';
				} else {
					*(*out)++ = 'I';
					*(*out)++ = c-0xff61+0x21;
				}
				*(*out)++ = '\033';
				*(*out)++ = '(';
				*(*out)++ = 'B';
				*outb -= 7;
				break;
			}
			c = uni_to_jis(c);
			if (!c) goto subst;
			if (*outb < 8) goto toobig;
			*(*out)++ = '\033';
			*(*out)++ = '$';
			*(*out)++ = 'B';
			*(*out)++ = c/256;
			*(*out)++ = c%256;
			*(*out)++ = '\033';
			*(*out)++ = '(';
			*(*out)++ = 'B';
			*outb -= 8;
			break;
		case UCS2:
			totype = UCS2BE;
		case UCS2BE:
		case UCS2LE:
		case UTF_16:
		case UTF_16BE:
		case UTF_16LE:
			if (c < 0x10000 || totype-UCS2BE < 2U) {
				if (c >= 0x10000) c = 0xFFFD;
				if (*outb < 2) goto toobig;
				put_16((void *)*out, c, totype);
				*out += 2;
				*outb -= 2;
				break;
			}
			if (*outb < 4) goto toobig;
			c -= 0x10000;
			put_16((void *)*out, (c>>10)|0xd800, totype);
			put_16((void *)(*out + 2), (c&0x3ff)|0xdc00, totype);
			*out += 4;
			*outb -= 4;
			break;
		case UTF_32:
			totype = UTF_32BE;
		case UTF_32BE:
		case UTF_32LE:
			if (*outb < 4) goto toobig;
			put_32((void *)*out, c, totype);
			*out += 4;
			*outb -= 4;
			break;
		}
	}
	*ploc = loc;
	return x;
ilseq:
	err = EILSEQ;
	x = -1;
	goto end;
toobig:
	err = E2BIG;
	x = -1;
	goto end;
starved:
	err = EINVAL;
	x = -1;
end:
	errno = err;
	*ploc = loc;
	return x;
}
