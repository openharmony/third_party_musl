## __RAWMEMCHR &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __rawmemchr - search for a character in the specified memory block.

#### **SYNOPSIS**​

       #include <string.h>

       void *__rawmemchr(const void *s, int c);


#### **DESCRIPTION**

​       The __rawmemchr() function is similar to memchr(): it assumes (i.e., the programmer knows for certain) that an instance of c lies somewhere in the memory area starting at the location  pointed to by s, and so performs an optimized search for c (i.e., no use of a count argument to limit the range of the search).  If an instance of c is not found, the results are unpredictable.  The following call is a fast means of locating a string's terminating null byte:

       char *p = __rawmemchr(s, '\0');

​       Note: This function may cause issues such as memory out of bounds access, and this interface has no validation.

#### **RETURN VALUE**

​       Returns the address of the first matching value c in the s space. If no matching byte is found, the result is unspecified.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __rawmemchr()           | Thread safety | MT-safe  |
|                         | Signal safety | Not safe |

#### HISTORY

​       -- 2025

#### CONFORMING TO

​      The __rawmemchr() function is a GNU extension.

#### EXAMPLES

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char *src = "hello world";
    char target = 'w';
    char *ret = __rawmemchr(src, target);
    if (ret > src + strlen(src) + 1) {
        printf("'%c' not found in '%s'.\n", target, src);
    } else {
        printf("Found '%c' at position: %td\n", target, ret - src);
    }
    return 0;
}
```

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)