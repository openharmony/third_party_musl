## __ATAN2_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __atan2_finite - arc tangent function of two variables.

#### **SYNOPSIS**

        #include <math.h>

        double __atan2_finite(double y, double x);

#### **DESCRIPTION**

​       This function calculate the principal value of the arc tangent of y/x, using the signs of the two arguments to determine the quadrant of the result.

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

​       C99, POSIX.1-2001, POSIX.1-2008.

​       The variant returning double also conforms to SVr4, 4.3BSD, C89.

#### EXAMPLES

```c
#include <stdio.h>
#include <math.h>

int main()
{
    double x = 2.0f;
    double y = 3.0f;
    double result = __atan2_finite(x, y);
    printf("log(%.1lf) = %.6lf\n", x, result);
    
    x = 4.0f;
    y = 0.5f;
    result = __atan2_finite(x, y);
    printf("log(%.1lf) = %.6lf\n", x, result);
    
    return 0;
}
```

#### COLOPHON

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).