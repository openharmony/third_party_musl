## GET_MUSL_VERSION &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       get_musl_version - get musl version.

#### **SYNOPSIS**​

       #include <musl_version.h>

       const char *get_musl_version(void);

#### **DESCRIPTION**

​       The get_musl_version() function gets the musl runtime version.

#### **RETURN VALUE**

​       The get_musl_version() function returns a string that identifies the musl runtime version in the format x.x.x(e.g., 1.2.5).

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

int main(int argc, char *argv[])
{
    const char *version = get_musl_version();  // version is 1.2.5
    if (version == NULL) {
        printf("Failed to get musl version.\n");
        return 1;
    }

    return 0;
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).