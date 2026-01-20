## __POWF_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual


#### **NAME**

​       __powf_finite - Compute x raised to the power of y for finite single-precision input values.

#### **SYNOPSIS**

       #include <math.h>

       float __powf_finite(float x, float y);

#### **DESCRIPTION**

​       The __powf_finite() function computes the value of x raised to the power of y (x^y) using single-precision floating-point arithmetic. This function is specifically designed to handle finite input values and provides optimized implementation for finite domain calculations with single-precision accuracy.

​       This function returns the value of x raised to the power of y. For finite input values, the result is well-defined and accurate.

#### **RETURN VALUE**
​       On success, this function return the value of x to the power of y.


#### **ERRORS**
​       See math_error for information on how to determine whether an error has occurred when calling this function.

​       The following errors can occur:

​       **Domain error**: x is negative, and y is a finite noninteger
              errno is set to EDOM.  An invalid floating-point exception (FE_INVALID) is raised.

​       **Pole error**: x is zero, and y is negative
              errno is set to ERANGE (but see BUGS).  A divide-by-zero floating-point exception (FE_DIVBYZERO) is raised.

​       **Range error**: the result overflows
              errno is set to ERANGE.  An overflow floating-point exception (FE_OVERFLOW) is raised.

​       **Range error**: the result underflows
              errno is set to ERANGE.  An underflow floating-point exception (FE_UNDERFLOW) is raised.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
|  __powf_finite() | Thread safety | MT-safe |
|                         | Signal safety | Not Safe     |

#### HISTORY

​       -- 2026

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any C standard.

#### EXAMPLES

```c
#include <math.h>
#include <stdio.h>

int main()
{
    float x = 2.0f;
    float y = 3.0f;
    float result = __powf_finite(x, y);
    printf("__powf_finite(%f, %f) = %f\n", x, y, result);
    
    x = 4.0f;
    y = 0.5f;
    result = __powf_finite(x, y);
    printf("__powf_finite(%f, %f) = %f\n", x, y, result);
    
    return 0;
}
```

#### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)