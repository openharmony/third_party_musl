## PTHREAD_SETCANCELTYPE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### NAME

​       pthread_setcanceltype - set the cancelability type of a thread.

#### SYNOPSIS

       #include <pthread.h>

       int pthread_setcanceltype(int type, int *oldtype);

#### DESCRIPTION

parameter `type`: the new cancelability type of the calling thread, which shall be one of the following:

​         •  PTHREAD_CANCEL_DEFERRED (0): cancellation requests are held until a cancellation point.

​         •  PTHREAD_CANCEL_ASYNCHRONOUS (1): cancellation requests are acted upon immediately.

parameter `oldtype`: if not NULL, receives the previous cancelability type of the calling thread.

The pthread_setcanceltype() function shall atomically set the calling thread's cancelability type to the indicated type and return the previous cancelability type at the location referenced by oldtype. The cancelability type determines whether cancellation requests received by the thread are acted upon immediately or are held until a cancellation point.

#### RETURN VALUE

On success, pthread_setcanceltype() returns 0. On error, an error number is returned.

When musl_extended_function is disabled, pthread_setcanceltype() returns ENOSYS.

#### ERRORS

The pthread_setcanceltype() function shall fail if:

​       **EINVAL**: The specified type is not PTHREAD_CANCEL_DEFERRED or PTHREAD_CANCEL_ASYNCHRONOUS.

The pthread_setcanceltype() function shall not return an error code of **EINTR**.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ------------------------| ------------- | -------- |
| pthread_setcanceltype() | Thread safety | MT-safe  |
|                         | Signal safety | Not Safe |

#### REFERS

Refer to [pthread_setcanceltype](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_setcanceltype.html).

#### EXAMPLES

```c
#include <pthread.h>
#include <stdio.h>

int main(void)
{
    int oldtype;
    int prevtype;

    /* save the current cancelability type and switch to asynchronous. */
    if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype) != 0) {
        printf("pthread_setcanceltype async failed\n");
        return -1;
    }
    printf("set asynchronous, previous type = %d\n", oldtype);

    /* ... do work requiring asynchronous cancellation ... */

    /* restore the previously saved cancelability type. */
    if (pthread_setcanceltype(oldtype, &prevtype) != 0) {
        printf("pthread_setcanceltype restore failed\n");
        return -1;
    }
    printf("restored type, previous (async) type = %d\n", prevtype);

    return 0;
}
```

#### NOTE

This interface is always declared. When `musl_extended_function` is disabled, pthread_setcanceltype() returns ENOSYS.

#### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).