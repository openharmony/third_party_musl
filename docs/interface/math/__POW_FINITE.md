## POWFINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __pow_finite - power functions.

#### **SYNOPSIS**

       #include <math.h> 

       double pow(double x, double y);

#### **DESCRIPTION**

​       These functions return the value of x raised to the power of y.

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

​       The following errors can occur:

​       **Domain error**: x is negative, and y is a finite noninteger<br>
​       errno is set to EDOM.  An invalid floating-point exception (FE_INVALID) is raised.

​       **Pole error**: x is zero, and y is negative<br>
​       errno is set to ERANGE (but see BUGS).  A divide-by-zero floating-point exception (FE_DIVBYZERO) is raised.

​       **Range error**: the result overflows<br>
​       errno is set to ERANGE.  An overflow floating-point exception (FE_OVERFLOW) is raised.

​       **Range error**: the result underflows<br>
​       errno is set to ERANGE.  An underflow floating-point exception (FE_UNDERFLOW) is raised.



#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __pow_finite()          | Thread safety | MT-safe  |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This function is ‌exclusively designed for PC environments‌.

#### CONFORMING TO

​       C99, POSIX.1-2001, POSIX.1-2008.<br>
​       The variant returning double also conforms to SVr4, 4.3BSD, C89.

#### EXAMPLES

```c
#include <stdio.h>
#include <math.h>

int main()
{
    float x = 4.0f;
    float y = 8.0f;
    float result = __pow_finite(x, y);
    printf("%f^%f = %f\n", x, y, result);
    
    x = -4.0f;
    y = 0.8f;
    result = __pow_finite(x, y);
    printf("%f^%f = %f\n", x, y, result);
    
    return 0;
}
```

#### COLOPHTON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)