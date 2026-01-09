## GET_MUSL_VERSION &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       get_musl_version - get musl version.

#### **SYNOPSIS**​

       #include <musl_version.h>

       const char *get_musl_version(void);

#### **DESCRIPTION**

​       The get_musl_version() function gets the musl runtime version.

#### **RETURN VALUE**

​       The get_musl_version() function returns a string that identifies the musl version available on the system.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| get_musl_version()      | Thread safety | MT-safe  |
|                         | Signal safety | Not safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      The function is musl-specific.

#### EXAMPLES

```c
#include <musl_version.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("musl version: %s\n", get_musl_version());
    exit(EXIT_SUCCESS);
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).