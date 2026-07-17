## __ATAN2_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __atan2_finite - arc tangent function of two variables.

#### **SYNOPSIS**

        #include <math.h>

        double __atan2_finite(double y, double x);

#### **DESCRIPTION**

​       The __atan2_finite() function calculates the principal value of the arc tangent of y/x, using the signs of the two arguments to determine the quadrant of the result.

#### **RETURN VALUE**

​       On success, this function returns the principal value of the arc tangent of y/x in radians, with the return value in the range [-π, π].Special value return rules:

       If either x or y is NaN, a NaN is returned.

       If y is +0 (-0) and x < 0, +π (-π) is returned.

       If y is +0 (-0) and x > 0, +0 (-0) is returned.

       If y is +0 (-0) and x is -0, +π (-π) is returned.
       
       If y is +0 (-0) and x is +0, +0 (-0) is returned.
    
       If y < 0 and x is +0/-0, -π/2 is returned; if y > 0 and x is +0/-0, π/2 is returned.
       
       If y is a non-zero finite value and x is -∞, +π (-π) is returned for y > 0 (y < 0); 
       
       If x is +∞, +0 (-0) is returned for y > 0 (y < 0).

       If y is +∞ (-∞) and x is a finite value, +π/2 (-π/2) is returned.

       If y is +∞ (-∞) and x is +∞, +π/4 (-π/4) is returned.

       If y is +∞ (-∞) and x is -∞, +3π/4 (-3π/4) is returned.

#### **ERRORS**

​       No errors occur.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __atan2_finite()        | Thread safety | MT-safe  |
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

int main()
{
    double y = 3.0;
    double x = 2.0;
    double result = __atan2_finite(y, x);
    printf("__atan2_finite(%.1lf, %.1lf) = %.6lf\n", y, x, result);
    
    y = 0.5;
    x = 4.0;
    result = __atan2_finite(y, x);
    printf("__atan2_finite(%.1lf, %.1lf) = %.6lf\n", y, x, result);
    
    return 0;
}
```

#### COLOPHON

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).