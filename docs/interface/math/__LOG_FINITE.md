## __LOG_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __log_finite - natural logarithmic function.

#### **SYNOPSIS**

       #include <math.h> 

       double __log_finite(double x);

#### **DESCRIPTION**

​       The __log_finite() function returns the natural logarithm of x.

#### **RETURN VALUE**

​       On success, this function return the natural logarithm of x.

​       If x is a NaN, a NaN is returned.

​       If x is 1, the result is +0.

​       If x is positive infinity, positive infinity is returned.

​       If x is zero, then a pole error occurs, and the function returns negative infinity (-INFINITY).

​       If x is negative (including negative infinity), then a domain error occurs, and a NaN (not a number) is returned.

#### **ERRORS**
​       See math_errhandling for information on how to determine whether an error has occurred when calling this function.

​       The following errors can occur:

​       **Domain error**: x is negative

​       An invalid floating-point exception (FE_INVALID) is raised. This implementation does not set errno for this case.

​       **Pole error**: x is zero

​       A divide-by-zero floating-point exception (FE_DIVBYZERO) is raised. This implementation does not set errno for this case.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __log_finite()          | Thread safety | MT-safe  |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​       This is a platform-specific extension and is not part of any C standard.

#### EXAMPLES

```c
#include <stdio.h>
#include <math.h>
#include <fenv.h>

int main()
{

    double x = 4.0;
    double result = __log_finite(x);
    printf("__log_finite(%.1lf) = %.6lf\n", x, result);

    x = 0.0;
    feclearexcept(FE_ALL_EXCEPT);
    result = __log_finite(x);
    printf("__log_finite(%.1lf) = %.6lf | FE_DIVBYZERO: %d\n",
           x, result, fetestexcept(FE_DIVBYZERO) != 0);

    x = -2.0;
    feclearexcept(FE_ALL_EXCEPT);
    result = __log_finite(x);
    printf("__log_finite(%.1lf) = %.6lf | FE_INVALID: %d\n",
           x, result, fetestexcept(FE_INVALID) != 0);
    
    return 0;
}
```

#### COLOPHON

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)