## __ACOSF_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual


#### **NAME**

​       __acosf_finite - Compute arc cosine of finite input value.

#### **SYNOPSIS**

       #include <math.h>

       float __acosf_finite(float x);

#### **DESCRIPTION**

​       The __acosf_finite() function computes the principal value of the arc cosine of x in the range [0, π] radians. This function is specifically designed to handle finite input values within the valid domain [-1, 1].

​       If x is outside the range [-1, 1], the behavior is undefined. 

#### **RETURN VALUE**
​       On success, these functions return the arc cosine of x in radians, the return value is in the range [0, π].

​       If x is a NaN, a NaN is returned.

​       If x is +1, +0 is returned.

​       If x is positive infinity or negative infinity, a domain error occurs, and a NaN is returned.

​       If x is outside the range [-1, 1], a domain error occurs, and a NaN is returned.

#### **ERRORS**
​       See math_error for information on how to determine whether an error has occurred when calling these functions.

​       The following errors can occur:

​       **Domain error**: x is outside the range [-1, 1]
              errno is set to EDOM.  An invalid floating-point exception (FE_INVALID) is raised.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
|  __acosf_finite() | Thread safety | MT-safe |
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
    float x = 0.5f;
    float result = __acosf_finite(x);
    printf("acosf(%f) = %f radians\n", x, result);
    printf("acosf(%f) = %f degrees\n", x, result * 180.0f / 3.14159265f);
    
    return 0;
}
```

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)