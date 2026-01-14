## __STRTOK_R &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __strtok_r - extract tokens from strings.

#### **SYNOPSIS**​

       #include <string.h>

       char *__strtok_r(char *restrict s, const char *restrict sep, char **restrict p);


#### **DESCRIPTION**

​       The   __strtok_r() function is same to strtok_r function. The s parameter can be NULL. When both s and *p are NULL, this interface will return NULL. The sep parameter cannot be NULL, otherwise it will cause segment errors in the strcspn function it calls.

#### **RETURN VALUE**

​       The __strtok_r() functions return a pointer to the next token, or NULL if there are no more tokens.


#### ATTRIBUTES

| Interface    | Attribute     | Value   |
| ------------ | ------------- | ------- |
| __strtok_r() | Thread safety | MT-safe |
|              | Signal safety | Not Safe|

#### HISTORY

​       -- 2026

#### NOTES

​       The first call to _strtok_r() should pass the string s and a NULL saveptr. Subsequent calls should pass NULL for sand the same saveptr to continue tokenization.

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      The __strtok_r() conforms to POSIX.1-2001, POSIX.1-2008.

#### EXAMPLES

```c
#include <stdio.h>
#include <string.h>

int main()
{
    char str[] = "hello,world,test";
    char *token;
    char *saveptr;

    token = __strtok_r(str, ",", &saveptr);
    printf("first token: %s\n", token);

    while ((token = __strtok_r(NULL, ",", &saveptr)) != NULL) {
        printf("next token: %s\n", token);
    }

    return 0;
}
```

#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).