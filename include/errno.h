#ifndef	_ERRNO_H
#define _ERRNO_H

#ifdef __ICCARM__ /* for iar */

#include <bits/errno.h>
#include_next <errno.h>

#else

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#include <bits/errno.h>

#ifdef __GNUC__
__attribute__((const))
#endif
int *__errno_location(void);
#define errno (*__errno_location())

#ifdef _GNU_SOURCE
extern char *program_invocation_short_name, *program_invocation_name;
#endif

#ifdef __cplusplus
}
#endif

#endif /* __ICCARM__ */
#endif

