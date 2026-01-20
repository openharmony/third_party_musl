## __LOG_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __log_finite - natural logarithmic function.

#### **SYNOPSIS**

       #include <math.h> 

       double __log_finite(double x);

#### **DESCRIPTION**

​       This function return the natural logarithm of x.

#### **RETURN VALUE**

​       On success, this function return the natural logarithm of x.

​       If x is a NaN, a NaN is returned.

​       If x is 1, the result is +0.

​       If x is positive infinity, positive infinity is returned.

​       If x is zero, then a pole error occurs, and the function returns negative infinity (-INFINITY).

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
| __log_finite()          | Thread safety | MT-safe  |
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
#include <errno.h>
#include <fenv.h>

int main()
{

    double x = 4.0;
    double result = __log_finite(x);
    printf("__log_finite(%.1lf) = %.6lf\n", x, result);

    double x = 0.0;
    errno = 0;
    result = __log_finite(x);
    printf("__log_finite(%.1lf) = %.6lf | errno: %d (ERANGE=%d)\n", 
           x, result, errno, ERANGE);

    double x = -2.0;
    errno = 0;
    result = __log_finite(x);
    printf("__log_finite(%.1lf) = %.6lf | errno: %d (EDOM=%d)\n", 
           x, result, errno, EDOM);
    
    return 0;
}

#### COLOPHON

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)