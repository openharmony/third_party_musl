#ifndef _STR_OPS_H
#define _STR_OPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

typedef struct _str_list_ {
  size_t num;     /** Number of entries in list */
  size_t size;  /** Storage size */
  char **strs;  /** string list */
} strlist;

strlist *strlist_alloc(size_t size);
void strlist_free(strlist *strs);

void strlwc(char *str);
size_t strtrim(char *str);
strlist *strsplit(const char *str, const char *split_s);
void strlist_set(strlist *strs, const char *str);

#ifdef __cplusplus
}
#endif

#endif
