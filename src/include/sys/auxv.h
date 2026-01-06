#ifndef SYS_AUXV_H
#define SYS_AUXV_H

#include "../../../include/sys/auxv.h"

#include <features.h>

#ifdef MUSL_EXTERNAL_FUNCTION
unsigned long __getauxval(unsigned long);
#else
hidden unsigned long __getauxval(unsigned long);
#endif

#endif
