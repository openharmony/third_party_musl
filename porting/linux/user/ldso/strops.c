#include <ctype.h>
#include "strops.h"

/* string to lower */
void strlwc(char *str)
{
    if (str == NULL) return;
    while (*str != '\0') {
        *str = (char)tolower(*str);
        str++ ;
    }
    return;
}
/* trim head and tail spaces of string */
size_t strtrim(char *str)
{
    char *last = NULL;
    char *dest = str;

    if (str == NULL) return 0;

    last = str + strlen(str);
    while (isspace((int)*str) && *str) str++;
    while (last > str) {
        if (!isspace((int)*(last-1))) break;
        last--;
    }
    *last = (char)0;

    memmove(dest, str, last-str+1);
    return last-str;
}
/* split string to list by given string */
strlist *strsplit(const char *str, const char *split_s)
{
    char *cur, *next;
    if(!str) return NULL;

    strlist *sl = strlist_alloc(0);
    char *ss = strdup(str);
    if (!sl || !ss) {
        strlist_free(sl);
        free(ss);
        return NULL;
    }

    cur = ss;
    while (next = strstr(cur, split_s)) {
        *next = 0;
        strtrim(cur);
        strlist_set(sl, cur);
        cur = next + strlen(split_s);
    }
    strtrim(cur);
    strlist_set(sl, cur);
    free(ss);
    return sl;
}

#define STR_DEFAULT_SIZE   16

strlist *strlist_alloc(size_t size)
{
    strlist *strs;
    if (size < STR_DEFAULT_SIZE) size = STR_DEFAULT_SIZE ;

    strs = (strlist *)calloc(1, sizeof *strs) ;

    if (strs) {
        strs->strs  = (char **)calloc(size, sizeof *strs->strs);
        if (strs->strs) {
            strs->size = size;
        } else {
            free(strs);
            strs = NULL;
        }
    }
    return strs ;
}

static void strlist_realloc(strlist *strs)
{
    if(!strs) return;
    size_t size = 2*strs->size;
    if (size) {
        char **ss = (char **)realloc(strs->strs, size * (sizeof *strs->strs));
        if (ss) {
            strs->size = size;
            strs->strs = ss;
        }
    }
    return;
}

void strlist_free(strlist *strs)
{
    if (!strs) return;
    for (size_t i=0; i < strs->num; i++) {
        free(strs->strs[i]);
    }
    free(strs->strs);
    free(strs);
}

void strlist_set(strlist *strs,const char *str)
{
    if (!strs || !str) return;
    if (strs->num == strs->size) {
       strlist_realloc(strs);
    }
    if (strs->num < strs->size) {
        strs->strs[strs->num] = strdup(str);
        if (strs->strs[strs->num]) strs->num++;
    }
}