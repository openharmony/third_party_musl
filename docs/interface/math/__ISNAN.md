## __ISNAN &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __isnan - BSD floating-point classification functions.

#### **SYNOPSIS**
       
       #include <math.h>

       int __isnan(double x);

#### **DESCRIPTION**

​       The __isnan() shall determine whether its argument value is a "not-a-number" (NaN) value.

#### **RETURN VALUE**

​       The __isnan() shall return a non-zero value if and only if its argument is a "not-a-number" (NaN) value, and 0 otherwise.

#### **ERRORS**

​       No errors are defined.

#### ATTRIBUTES

| Interface    | Attribute     | Value    |
| -------------| ------------- | -------- |
| __isnan()   | Thread safety  | MT-safe  |
|              | Signal safety | Not Safe |

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

int main(void)
{
    float data = 0.0f / 0.0f;
    int ret = __isnan(data);
    if (ret == 1) {
        printf("data is NaN, ret: %d\n", ret);
    } else {
        printf("data is not NaN");
    }
    return 0;
}
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)