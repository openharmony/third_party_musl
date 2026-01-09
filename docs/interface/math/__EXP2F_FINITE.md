## __EXP2F_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual


#### **NAME**

​       __exp2f_finite - Compute 2 raised to the power of finite single-precision input value.

#### **SYNOPSIS**

       #include <math.h>

       float __exp2f_finite(float x);

#### **DESCRIPTION**

​       The __exp2f_finite() function computes the value of 2 raised to the power of x (2^x) using single-precision floating-point arithmetic. This function is specifically designed to handle finite input values and provides optimized implementation for finite domain calculations with single-precision accuracy.

​       This function returns the exponential value of 2 raised to the power of x. For finite input values, the result is well-defined and accurate.

#### **RETURN VALUE**
​       On success, these functions return the base-2 exponential value of x.

​       For various special cases, including the handling of infinity and NaN, as well as overflows and underflows, see exp.

#### **ERRORS**
​       See math_error for information on how to determine whether an error has occurred when calling these functions.

​       For a discussion of the errors that can occur for these functions, see exp.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
|  __exp2f_finite() | Thread safety | MT-safe |
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
    float x = 3.0f;
    float result = __exp2f_finite(x);
    printf("2^%f = %f\n", x, result);
    
    x = -1.5f;
    result = __exp2f_finite(x);
    printf("2^%f = %f\n", x, result);
    
    return 0;
}
```

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)