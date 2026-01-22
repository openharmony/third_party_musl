## __LOG10_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __log10_finite - base 10 logarithmic function.

#### **SYNOPSIS**

       #include <math.h> 

       double __log10_finite(double x);

#### **DESCRIPTION**

​       ​This function return the base 10 logarithm of x.

#### **RETURN VALUE**

​       On success, this function return the base 10 logarithm of x.

​       If x is a NaN, a NaN is returned.

​       If x is 1, the result is +0.

​       If x is positive infinity, positive infinity is returned.

​       If x is zero, then a pole error occurs, and the functions return -HUGE_VAL, -HUGE_VALF, or -HUGE_VALL, respectively.

​       If x is negative (including negative infinity), then a domain error occurs, and a NaN (not a number) is returned.

#### **ERRORS**

​       The following errors can occur:

​       **Domain error**: x is negative

​       errno is set to EDOM.  An invalid floating-point exception (FE_INVALID) is raised.

​       **Pole error**: x is zero

​       errno is set to ERANGE.  A divide-by-zero floating-point exception (FE_DIVBYZERO) is raised.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __log10_finite()        | Thread safety | MT-safe  |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​       C99, POSIX.1-2001, POSIX.1-2008.

​       The variant returning double also conforms to SVr4, 4.3BSD, C89.

#### EXAMPLES

```c
#include <stdio.h>
#include <math.h>

int main()
{
    double x = 4.0;
    double result = __log10_finite(x);
    printf("__log10_finite(%.1lf) = %.6lf\n", x, result);

    x = 0.7;
    result = __log10_finite(x);
    printf("__log10_finite(%.1lf) = %.6lf\n", x, result);
    
    return 0;
}
```

#### COLOPHON

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)