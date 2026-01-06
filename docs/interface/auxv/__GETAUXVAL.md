## __GETAUXVAL &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __getauxval - retrieve a value from the auxiliary vector.

#### **SYNOPSIS**   
       
       #include <sys/auxv.h>

       unsigned long __getauxval(unsigned long item);

#### **DESCRIPTION**

​       The __getauxval() function is similar to getauxval.

#### **RETURN VALUE**

​       On success, __getauxval() returns the value corresponding to type.  If type is not found, 0 is returned. 

#### **ERRORS**

​       **ENOENT**: No entry corresponding to type could be found in the auxiliary vector.

#### ATTRIBUTES

| Interface     | Attribute     | Value    |
| ------------- | ------------- | -------- |
| __getauxval() | Thread safety | MT-safe  |
|               | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This function is ‌exclusively designed for PC environments‌.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <sys/auxv.h>
#include <stdio.h>

int main(void) {
    unsigned long int result;

    result = __getauxval(AT_SECURE);
    printf("__getauxval call succeeded: AT_SECURE = %lu → %s\n", 
           result, 
           result == 0 ? "non-secure mode" : "secure mode");

    result = __getauxval(AT_RANDOM);
    printf("__getauxval call succeeded: AT_RANDOM = %lu → %s\n", 
           result, 
           result == 0 ? "no random buffer" : "pointer to 16-byte random buffer");

    result = __getauxval(0xFFFF); // Invalid auxv type
    if (result == 0) {
        printf("__getauxval call: invalid type 0x%x → returns 0 (type not found)\n", 0xFFFF);
    }
    
    return 0;
}
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)