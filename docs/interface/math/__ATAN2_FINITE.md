## ATAN2FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __atan2_finite - arc tangent function of two variables.

#### **SYNOPSIS**

       #include <malloc.h> 

       int malloc_check_from_ptr(void *ptr);

#### **DESCRIPTION**

​       These functions calculate the principal value of the arc tangent of y/x, using the signs of the two arguments to determine the quadrant of the result.

#### **RETURN VALUE**

​       On success, these functions return the principal value of the arc tangent of y/x in radians; the return value is in the range [-pi, pi].

​       If y is +0 (-0) and x is less than 0, +pi (-pi) is returned.

​       If y is +0 (-0) and x is greater than 0, +0 (-0) is returned.

​       If y is less than 0 and x is +0 or -0, -pi/2 is returned.

​       If y is greater than 0 and x is +0 or -0, pi/2 is returned.

​       If either x or y is NaN, a NaN is returned.

​       If y is +0 (-0) and x is -0, +pi (-pi) is returned.

​       If y is +0 (-0) and x is +0, +0 (-0) is returned.

​       If y is a finite value greater (less) than 0, and x is negative infinity, +pi (-pi) is returned.

​       If y is a finite value greater (less) than 0, and x is positive infinity, +0 (-0) is returned.

​       If y is positive infinity (negative infinity), and x is finite, pi/2 (-pi/2) is returned.

​       If y is positive infinity (negative infinity) and x is negative infinity, +3*pi/4 (-3*pi/4) is returned.

​       If y is positive infinity (negative infinity) and x is positive infinity, +pi/4 (-pi/4) is returned.

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

​       C99, POSIX.1-2001, POSIX.1-2008.<br>

​       The variant returning double also conforms to SVr4, 4.3BSD, C89.

#### EXAMPLES

```c
#include <stdio.h>
#include <math.h>

int main()
{
    float x = 2.0f;
    float y = 3.0f;
    float result = __atan2_finite(x, y);
    printf("%f^%f = %f\n", x, y, result);
    
    x = 4.0f;
    y = 0.5f;
    result = __atan2_finite(x, y);
    printf("%f^%f = %f\n", x, y, result);
    
    return 0;
}
```

#### COLOPHTON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).