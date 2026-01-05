## __LOG2F_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual


#### **NAME**

​       __log2f_finite - Compute base-2 logarithm of finite single-precision input value.

#### **SYNOPSIS**

       #include <math.h>

       float __log2f_finite(float x);

#### **DESCRIPTION**

​       The __log2f_finite() function computes the base-2 logarithm of x using single-precision floating-point arithmetic. This function is specifically designed to handle finite input values greater than 0 and provides optimized implementation for finite domain calculations with single-precision accuracy.

​       This function returns the logarithm of x to the base 2. For finite input values greater than 0, the result is well-defined and accurate.

#### **RETURN VALUE**
​       On success, these functions return the base 2 logarithm of x.

​       For special cases, including where x is 0, 1, negative, infinity, or NaN, see log.
#### **ERRORS**
​       See math_error for information on how to determine whether an error has occurred when calling these functions.

​       For a discussion of the errors that can occur for these functions, see log.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
|  __log2f_finite() | Thread safety | MT-safe |
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
    float x = 8.0f;
    float result = __log2f_finite(x);
    printf("log2(%f) = %f\n", x, result);
    
    x = 0.5f;
    result = __log2f_finite(x);
    printf("log2(%f) = %f\n", x, result);
    
    return 0;
}
```

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)