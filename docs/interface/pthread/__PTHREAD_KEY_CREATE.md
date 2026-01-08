## __PTHREAD_KEY_CREATE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __pthread_key_create - thread-specific data key creation.

#### **SYNOPSIS**   
       
       #include <pthread.h>

       int __pthread_key_create(pthread_key_t *key, void (*destr)(void *));

#### **DESCRIPTION**

​       The __pthread_key_create() function is similar to pthread_key_create.

#### **RETURN VALUE**

​       If successful, the __pthread_key_create() function shall store the newly created key value at *key and shall return zero. Otherwise, an error number shall be returned to indicate the error.

#### **ERRORS**

​       __pthread_key_create() returns the following error code on error:  
​       **EAGAIN**: PTHREAD_KEYS_MAX keys are already allocated.  

#### ATTRIBUTES

| Interface              | Attribute     | Value     |
| ---------------------- | ------------- | --------- |
| __pthread_key_create() | Thread safety | MT-safe   |
|                        | Signal safety | Not Safe  |

#### HISTORY

​       -- 2026 

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static pthread_key_t g_key;

void *threadfuncB(void *arg)
{
    (void)arg;
    int32_t value = 0;
    int32_t ret = pthread_setspecific(g_key, &value);
    if (ret == 0) {
        printf("pthread_setspecific success : ret = %d\n", ret);
    } else {
        printf("pthread_setspecific fail : ret = %d\n", ret);
    }
    int32_t *keyRet = (int32_t *)pthread_getspecific(g_key);
    if (keyRet != NULL && *keyRet == 0) {
        printf("pthread_getspecific success : value = %d\n", *keyRet);
    } else {
        printf("pthread_getspecific fail : value = %d\n", (keyRet ? *keyRet : -1));
    }
    return arg;
}

void testfunc(void *arg)
{
    (void)arg;
}

int main(void) {
    pthread_t tid2;
    int32_t ret = __pthread_key_create(&g_key, testfunc);
    if (ret == 0) {
        printf("__pthread_key_create success : ret = %d\n", ret);
    } else {
        printf("__pthread_key_create fail : ret = %d\n", ret);
    }
    pthread_create(&tid2, NULL, threadfuncB, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)