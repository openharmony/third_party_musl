## STRTOLL_L &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual

#### **NAME**

​      strtoll_l - convert a string to a long long integer.

#### **SYNOPSIS**

```
    #include <stdlib.h>
    #include <locale.h>
    long long strtoll_l(const char *nptr, char **endptr, int base, locale_t loc);
```      

#### **DESCRIPTION**

​      The strtoll_l() function converts the initial part of the string in nptr to a long long integer value according to the given base, which must be between 2 and 36 inclusive, or be the special value 0.

​      The string may begin with an arbitrary amount of white space (as determined by isspace()) followed by a single optional '+' or '-' sign. If base is zero or 16, the string may then include a "0x" or "0X" prefix, and the number will be read in base 16; otherwise, a zero base is taken as 10 (decimal) unless the next character is '0', in which case it is taken as 8 (octal).

​      The remainder of the string is converted to a long long value in the obvious manner, stopping at the first character which is not a valid digit in the given base. (In bases above 10, the letter 'A' in either uppercase or lowercase represents 10, 'B' represents 11, and so forth, with 'Z' representing 35.)

​      If endptr is not NULL, strtoll_l() stores the address of the first invalid character in *endptr. If there were no digits at all, strtoll_l() stores the original value of nptr in *endptr (and returns 0). In particular, if *nptr is not '\0' but **endptr is '\0' on return, the entire string is valid.

​      Locale object (locale_t) specifying the locale for parsing (e.g., handling localized thousands separators like , or .).    

#### **RETURN VALUE**

​      On success, returns the converted long long value.

​      If no valid conversion is possible, returns 0 and sets errno to EINVAL.

​      If the value overflows LLONG_MAX/LLONG_MIN, returns LLONG_MAX/LLONG_MIN and sets errno to ERANGE.

#### **ERRORS**

​      The following error codes may be set in errno:

​      **​EINVAL**：The given base contains an unsupported value.

​      **ERANGE**：The resulting value was out of range.

#### ATTRIBUTES

| Interface   | Attribute     | Value          |
| ----------- | ------------- | -------------- |
| strtoll_l() | Thread safety | MT-Safe locale |

#### HISTORY

​      -- 2025 

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      POSIX.1-2001, POSIX.1-2008, C99.

#### EXAMPLES

```c
#define _GNU_SOURCE
#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <errno.h>

int main() {
    const char *str = "1234";
    char *endptr;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return 1;
    }
    long long num = strtoll_l(str, &endptr, 0, loc);

    // num = 1234, endptr points to the end of the string
    printf("num = %lld endptr-str = %ld\n", num, endptr-str);

    freelocale(loc);
    return 0;
}
```

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)