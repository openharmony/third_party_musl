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
​       On success, these functions return the value of x to the power of y.

​       If x is a finite value less than 0, and y is a finite noninteger, a domain error occurs, and a NaN is returned.

​       If the result overflows, a range error occurs, and the functions return HUGE_VAL, HUGE_VALF, or HUGE_VALL, respectively, with the mathematically correct sign.

​       If result underflows, and is not representable, a range error occurs, and 0.0 is returned.

​       Except as specified below, if x or y is a NaN, the result is a NaN.

​       If x is +1, the result is 1.0 (even if y is a NaN).

​       If y is 0, the result is 1.0 (even if x is a NaN).

​       If x is +0 (-0), and y is an odd integer greater than 0, the result is +0 (-0).

​       If x is 0, and y greater than 0 and not an odd integer, the result is +0.

​       If x is -1, and y is positive infinity or negative infinity, the result is 1.0.

​       If the absolute value of x is less than 1, and y is negative infinity, the result is positive infinity.

​       If the absolute value of x is greater than 1, and y is negative infinity, the result is +0.

​       If the absolute value of x is less than 1, and y is positive infinity, the result is +0.

​       If the absolute value of x is greater than 1, and y is positive infinity, the result is positive infinity.

​       If x is negative infinity, and y is an odd integer less than 0, the result is -0.

​       If x is negative infinity, and y less than 0 and not an odd integer, the result is +0.

​       If x is negative infinity, and y is an odd integer greater than 0, the result is negative infinity.

​       If x is negative infinity, and y greater than 0 and not an odd integer, the result is positive infinity.

​       If x is positive infinity, and y less than 0, the result is +0.

​       If x is positive infinity, and y greater than 0, the result is positive infinity.

​       If x is +0 or -0, and y is an odd integer less than 0, a pole error occurs and HUGE_VAL, HUGE_VALF, or HUGE_VALL, is returned, with the same sign as x.

​       If x is +0 or -0, and y is less than 0 and not an odd integer, a pole error occurs and +HUGE_VAL, +HUGE_VALF, or +HUGE_VALL, is returned.

#### **ERRORS**
​       See math_error for information on how to determine whether an error has occurred when calling these functions.

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

​       This function is ‌exclusively designed for PC environments‌.

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
    printf("%f^%f = %f\n", x, y, result);
    
    x = 4.0f;
    y = 0.5f;
    result = __powf_finite(x, y);
    printf("%f^%f = %f\n", x, y, result);
    
    return 0;
}
```

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)