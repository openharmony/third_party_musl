## __POW_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __pow_finite - power function.

#### **SYNOPSIS**

        #include <math.h>

        double __pow_finite(double x, double y);

#### **DESCRIPTION**

​       This function return the value of x raised to the power of y.

#### **RETURN VALUE**

​       On success, this function return the value of x to the power of y.


#### **ERRORS**

​       The following errors can occur:

​       **Domain error**: x is negative, and y is a finite noninteger

​       errno is set to EDOM.  An invalid floating-point exception (FE_INVALID) is raised.

​       **Pole error**: x is zero, and y is negative

​       errno is set to ERANGE (but see BUGS).  A divide-by-zero floating-point exception (FE_DIVBYZERO) is raised.

​       **Range error**: the result overflows

​       errno is set to ERANGE.  An overflow floating-point exception (FE_OVERFLOW) is raised.

​       **Range error**: the result underflows

​       errno is set to ERANGE.  An underflow floating-point exception (FE_UNDERFLOW) is raised.



#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __pow_finite()          | Thread safety | MT-safe  |
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
    double x = 4.0f;
    double y = 8.0f;
    double result = __pow_finite(x, y);
    printf("__pow_finite(%.2lf, %.2lf) = %.2lf\n", x, y, result);
    
    x = -4.0f;
    y = 0.8f;
    result = __pow_finite(x, y);
    printf("__pow_finite(%.2lf, %.2lf) = %.2lf\n", x, y, result);
    
    return 0;
}
```

#### COLOPHON

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)