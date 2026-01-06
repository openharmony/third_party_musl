## EXPFINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __exp_finite - base-e exponential function.

#### **SYNOPSIS**

       #include <math.h> 

       double __exp_finite(double x);

#### **DESCRIPTION**

​       These functions return the value of e (the base of natural logarithms) raised to the power of x.

#### **RETURN VALUE**

​       On success, these functions return the exponential value of x.

​       If x is a NaN, a NaN is returned.

​       If x is positive infinity, positive infinity is returned.

​       If x is negative infinity, +0 is returned.

​       If the result underflows, a range error occurs, and zero is returned.

​       If the result overflows, a range error occurs, and the functions return +HUGE_VAL, +HUGE_VALF, or +HUGE_VALL, respectively.

#### **ERRORS**

​       The following errors can occur:

​       **Range error**: overflow
​       errno is set to ERANGE.  An overflow floating-point exception (FE_OVERFLOW) is raised.

​       **Range error**: underflow
​       errno is set to ERANGE.  An underflow floating-point exception (FE_UNDERFLOW) is raised.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __exp_finite()          | Thread safety | MT-safe  |
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
int main()
{
    float x = 8.0f;
    float result = __exp_finite(x);
    printf("exp(%f) = %f\n", x, result);
    
    x = 0.5f;
    result = __exp_finite(x);
    printf("exp(%f) = %f\n", x, result);
    
    return 0;
}
```

#### COLOPHTON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).