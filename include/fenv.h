#ifndef _FENV_H
#define _FENV_H

#include <bits/fenv.h>

#ifdef __cplusplus
extern "C" {
#endif

int feclearexcept(int);
int fegetexceptflag(fexcept_t *, int);
int feraiseexcept(int);
int fesetexceptflag(const fexcept_t *, int);
int fetestexcept(int);

int fegetround(void);
int fesetround(int);

int fegetenv(fenv_t *);
int feholdexcept(fenv_t *);
int fesetenv(const fenv_t *);
int feupdateenv(const fenv_t *);

#if defined(__aarch64__)
// musl supports for Python-related components
int fegetexcept(void);
int feenableexcept(int);
int fedisableexcept(int);
#endif

#ifdef __cplusplus
}
#endif
#endif

