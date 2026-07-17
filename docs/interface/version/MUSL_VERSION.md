## GET_MUSL_VERSION &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       get_musl_version - get musl version.

#### **SYNOPSIS**​

       #include <musl_version.h>

       const char *get_musl_version(void);

#### **DESCRIPTION**

​       The get_musl_version() function gets the musl runtime version.

#### **RETURN VALUE**

​       The get_musl_version() function returns the musl runtime version string generated at build time. The string is typically in x.x.x format (for example, 1.2.6), but may include build or git suffixes depending on how the version header is generated.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| get_musl_version()      | Thread safety | MT-safe  |
|                         | Signal safety | Safe     |

#### HISTORY

​       -- 2026

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      The function is musl-specific.

#### EXAMPLES

```c
#include <musl_version.h>
#include <stdio.h>

int main(void)
{
    const char *version = get_musl_version();
    printf("musl version: %s\n", version);

    return 0;
}
```


#### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).