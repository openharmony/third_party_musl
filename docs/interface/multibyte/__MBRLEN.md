## __MBRLEN &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual

#### **NAME**

​      __mbrlen - determine number of bytes in next multibyte character

#### **SYNOPSIS**

```
    #include <wchar.h>    
    size_t __mbrlen(const char *s, size_t n, mbstate_t *ps);
```

#### **DESCRIPTION**

​      The __mbrlen() function inspects at most n bytes of the multibyte string starting at s and extracts the next complete multibyte character. It updates the shift state *ps. 
​      If the multibyte character is not the null wide character, it returns the number of bytes that were consumed from s. If the multibyte character is the null wide character, it resets the shift state *ps to the initial state and returns 0.

​      If the n bytes starting at s do not contain a complete multibyte character, __mbrlen() returns (size_t) -2. This can happen even if n >= MB_CUR_MAX, if the multibyte string contains redundant shift sequences.

​      If the multibyte string starting at s contains an invalid multibyte sequence before the next complete character, __mbrlen() returns (size_t) -1 and sets errno to EILSEQ. In this case, the effects on *ps are undefined.

​      If ps is NULL, a static anonymous state known only to the __mbrlen() function is used instead.

#### **RETURN VALUE**

​      The __mbrlen() function returns the number of bytes parsed from the multibyte sequence starting at s, if a non-null wide character was recognized. It returns 0, if a null wide character was recognized. It returns (size_t) -1 and sets errno to EILSEQ, if an invalid multibyte sequence was encountered. It returns (size_t) -2 if it couldn't  parse a complete multibyte character, meaning that n should be increased.

#### **ERRORS**

​      The following error codes may be set in errno:

​      ​**EILSEQ**: Illegal byte sequence.

#### ATTRIBUTES

| Interface  | Attribute     | Value        |
| ---------- | ------------- | ------------ |
| __mbrlen() | Thread safety | MT-Unsafe    |

#### HISTORY

​      -- 2026

#### NOTES

​      The behavior of __mbrlen() depends on the LC_CTYPE category of the current locale.

#### CONFORMING TO

​      POSIX.1-2001, POSIX.1-2008, C99.

#### EXAMPLES

```c
#include <locale.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    char mb_str[] = {0xE4, 0xB8, 0xAD};
    mbstate_t state = {0};
    size_t len = __mbrlen(mb_str, MB_CUR_MAX, &state);
    printf("len: %zu\n", len);
    return 0;
}
```

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)