## __isnanf &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __isnanf - Determine whether a float number is a "not-a-number" (NaN) value.

#### **SYNOPSIS**

       ​#include <math.h>

       int __isnanf(float x);

#### **DESCRIPTION**

​       The __isnanf() shall determine whether its argument value is a "not-a-number" (NaN) value. 

#### **RETURN VALUE**

​       The __isnanf() shall return a non-zero value if and only if its argument is a "not-a-number" (NaN) value, and 0 otherwise.

#### **ERRORS**

​       No errors are defined.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __isnanf() | Thread safety | MT-Safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This function is ‌exclusively designed for PC environments‌.

#### CONFORMING TO

​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <float.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    float f = 50.0f;

    printf("The result is : %d\n", __isnanf(HUGE_VALF)); // The result is : 0
    printf("The result is : %d\n", __isnanf(NAN)); // The result is : 1
    printf("The result is : %d\n", __isnanf(0)); // The result is : 0
    printf("The result is : %d\n", __isnanf(f)); // The result is : 0

    return 0;
}
```

#### COLOPHTON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)