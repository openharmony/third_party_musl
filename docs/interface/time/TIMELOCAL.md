## TIMELOCAL &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​      timelocal - inverses of gmtime and localtime.

#### **SYNOPSIS**

```
    ​#include <time.h>
    time_t timelocal(struct tm *tm);
```

#### **DESCRIPTION**

​      The functions timelocal() is the inverses of localtime(). function take a broken-down time and convert it to calendar time (seconds since ​the Epoch, 1970-01-01 00:00:00 +0000, UTC). timelocal() takes the local timezone into account when doing the conversion.

#### **RETURN VALUE**

​      On success, return the calendar time (seconds since the Epoch), expressed as a value of type time_t.

​      On error, return the value (time_t) -1 and set errno to indicate the cause of the error.

#### **ERRORS**

​      The following error codes may be set in errno:

​      ​**EOVERFLOW**: The result cannot be represented.

#### ATTRIBUTES

| Interface     | Attribute     | Value        |
| ------------- | ------------- | ------------ |
| timelocal()   | Thread safety | MT-Unsafe    |

#### HISTORY

​      -- 2026 

#### NOTES

​      The timelocal() function is equivalent to the POSIX standard function mktime(). There is no reason to ever use it.

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      These functions are nonstandard GNU extensions that are also present on the BSDs. Avoid their use.

#### EXAMPLES

```c
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    struct tm tm_info = {
        .tm_year = 2025 - 1900,
        .tm_mon = 12,
        .tm_mday = 4,
        .tm_hour = 15,
        .tm_min = 30,
        .tm_sec = 0,
        .tm_isdst = -1
    };

    time_t timestamp = timelocal(&tm_info);
    if (timestamp == -1) {
        perror("timelocal failed");
        return 1;
    }

    printf("timestamp: %lld\n", (long long)timestamp);
    printf("localtime: %s", asctime(&tm_info));

    return 0;
}

```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)