## __EXP_FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __exp_finite - base-e exponential function.

#### **SYNOPSIS**

       #include <math.h> 

       double __exp_finite(double x);

#### **DESCRIPTION**

​       This function return the value of e (the base of natural logarithms) raised to the power of x.

#### **RETURN VALUE**

​       On success, this function return the exponential value of x.


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

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​       C99, POSIX.1-2001, POSIX.1-2008.

​       The variant returning double also conforms to SVr4, 4.3BSD, C89.

#### EXAMPLES

```c
int main()
{
    double x = 8.0f;
    double result = __exp_finite(x);
    printf("__exp_finite(%.2lf) = %.2lf\n", x, result);
    
    x = 0.5f;
    result = __exp_finite(x);
    printf("__exp_finite(%.2lf) = %.2lf\n", x, result);
    return 0;
}
```

#### COLOPHON

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).