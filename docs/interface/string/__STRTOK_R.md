## __STRTOK_R &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __strtok_r - extract tokens from strings.

#### **SYNOPSIS**​

       #include <string.h>

       char *__strtok_r(char *restrict s, const char *restrict sep, char **restrict p);


#### **DESCRIPTION**

​       The   __strtok_r() function is same to strtok_r function.

#### **RETURN VALUE**

​       The __strtok_r() functions return a pointer to the next token, or NULL if there are no more tokens.


#### ATTRIBUTES

| Interface    | Attribute     | Value   |
| ------------ | ------------- | ------- |
| __strtok_r() | Thread safety | MT-safe |
|              | Signal safety | Not Safe|

#### HISTORY

​       -- 2025

#### NOTES

​       On some implementations, *saveptr is required to be NULL on the first call to __strtok_r() that is being used to parse str.

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

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)