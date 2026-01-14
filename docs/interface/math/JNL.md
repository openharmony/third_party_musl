## JNL &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       jnl - Bessel functions of the first kind.

#### **SYNOPSIS**
       
       #include <math.h>

       long double jnl(int n, long double x);

#### **DESCRIPTION**

​       The jnl() functions is similar to jn, are versions that take and return long double values.

#### **RETURN VALUE**

​       On success, these functions return the appropriate Bessel value of the first kind for x.

​       If x is a NaN, a NaN is returned.

​       If x is too large in magnitude, or the result underflows, a range error occurs, and the return value is 0.

#### **ERRORS**

​       The following errors can occur:  
​       **Range error**: result underflow, or x is too large in magnitude.

#### ATTRIBUTES

| Interface | Attribute     | Value    |
| ----------| ------------- | -------- |
| jnl()     | Thread safety | MT-safe  |
|           | Signal safety | Not Safe |

#### HISTORY

​       -- 2026 

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      The functions returning double conform to SVr4, 4.3BSD, POSIX.1-2001, and POSIX.1-2008.  The others are nonstandard functions that also exist on the BSDs.

#### EXAMPLES

```c
#include <stdio.h>
#include <math.h>

int main(void) {
    int n = 0;
    long double x = 2.0L;
    long double ret = jnl(n, x);
    printf("Normal value: jnl(%d, %.1Lf) = %Lg\n", n, x, ret);

    n = 0;
    x = -1.0L;
    ret = jnl(n, x);
    printf("Negative  value: jnl(%d, %.1Lf) = %Lg\n", n, x, ret);
    return 0;
}
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)