## __FINITE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __finite - BSD floating-point classification functions.

#### **SYNOPSIS**
       
       #include <math.h>

       int __finite(double x);

#### **DESCRIPTION**

​       The __finite() shall determine whether its argument value is a finite value. 

#### **RETURN VALUE**

​       The __finite() shall return a non-zero value if and only if its argument is neither infinite nor a "not-a-number" (NaN) value, and 0 otherwise.

#### **ERRORS**

​       No errors are defined.

#### ATTRIBUTES

| Interface      | Attribute     | Value    |
| ---------------| ------------- | -------- |
| __finite()     | Thread safety | MT-safe  |
|                | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This function is ‌exclusively designed for PC environments‌.

#### CONFORMING TO

​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <math.h>
#include <float.h>
#include <stdio.h>

int main(void) {
    double a = 0.0;
    double b = INFINITY;
    int result = __finite(a);
    if(result == 1) {
        printf("a is a finite value\n");
    } else {
        printf("a is not a finite value\n");
    }
    int result1 = __finite(b);
    if(result1 == 1) {
        printf("b is a finite value\n");
    } else {
        printf("b is not a finite value\n");
    }
    return 0;
}
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)