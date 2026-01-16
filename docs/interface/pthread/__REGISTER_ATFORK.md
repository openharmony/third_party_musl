## __REGISTER_ATFORK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __register_atfork - register fork handlers.

#### **SYNOPSIS**   
       
       #include <pthread.h>

       int __register_atfork(void (*prepare) (void), void (*parent) (void), void (*child) (void), void *dso_handle);

#### **DESCRIPTION**

​       The __register_atfork() function is similar to pthread_atfork, registers fork handlers that are to be executed when fork is called by any thread in a process. The handlers are executed in the context of the thread that calls fork.

​       Four kinds of handler can be registered:  

​         •  prepare specifies a handler that is executed in the parent process before fork processing starts.

​         •  parent specifies a handler that is executed in the parent process after fork processing completes.

​         •  child specifies a handler that is executed in the child process after fork processing completes.

​         •  dso_handle is an internal variable with no practical significance and should be passed directly as a null value.

​        If the corresponding stage in fork processing does not require a handler, any of the first three parameters can be NULL.

#### **RETURN VALUE**

​       On success, __register_atfork() returns zero.  On error, it returns an error number.  __register_atfork() may be called multiple times by a process to register additional handlers.  The handlers for each phase are called in a specified order: the prepare handlers are called in reverse order of registration; the parent and child handlers are called in the order of registration.

#### **ERRORS**

​       The __register_atfork() function shall fail if:  
​       **ENOMEM**: Insufficient memory exists to record the fork handler functions.

#### ATTRIBUTES

| Interface           | Attribute     | Value    |
| ------------------- | ------------- | -------- |
| __register_atfork() | Thread safety | MT-safe  |
|                     | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.​

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#define TEST_THREAD_SLEEP 3
pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sync_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sync_cond = PTHREAD_COND_INITIALIZER;
static int g_thread_locked = 0;
static int gprepare_flag = 0;
static int gparent_flag = 0;
static int gchild_flag = 0;

void reset_data(void)
{
    gprepare_flag = 0;
    gparent_flag = 0;
    gchild_flag = 0;
    g_thread_locked = 0;
}

static void prepare(void)
{
    int ret = pthread_mutex_lock(&glock);
    if (ret == 0) {
        printf("prepare callback: lock success\n");
        gprepare_flag++;
    } else {
        printf("prepare callback: lock fail, ret = %d\n", ret);
    }
}

static void parent(void)
{
    int ret = pthread_mutex_unlock(&glock);
    if (ret == 0) {
        printf("parent callback: unlock success\n");
        gparent_flag++;
    } else {
        printf("parent callback: unlock fail, ret = %d\n", ret);
    }
}

static void child(void)
{
    int ret = pthread_mutex_unlock(&glock);
    if (ret == 0) {
        printf("child callback: unlock success\n");
        gchild_flag++;
    } else {
        printf("child callback: unlock fail, ret = %d\n", ret);
    }
}

static void *thread_fun(void *arg)
{
    pthread_mutex_lock(&glock);
    printf("Child thread: successfully acquired glock\n");

    pthread_mutex_lock(&sync_lock);
    g_thread_locked = 1;
    pthread_cond_signal(&sync_cond);
    pthread_mutex_unlock(&sync_lock);

    sleep(TEST_THREAD_SLEEP);
    pthread_mutex_unlock(&glock);
    printf("Child thread: released glock\n");
    return NULL;
}

int main(void) {
    reset_data();
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    printf("pthread_create: ret=%d\n", ret);
    if (ret != 0) {
        printf("pthread_create fail\n");
        return -1;
    }

    ret = __register_atfork(prepare, parent, child, NULL);
    printf("__register_atfork: ret=%d\n", ret);
    if (ret != 0) {
        printf("__register_atfork fail\n");
        return -1;
    }

    pthread_mutex_lock(&sync_lock);
    while (g_thread_locked == 0) {
        printf("Main thread: waiting for child thread to acquire glock...\n");
        pthread_cond_wait(&sync_cond, &sync_lock);
    }
    pthread_mutex_unlock(&sync_lock);
    printf("Main thread: confirmed child thread has acquired glock, start executing fork\n");

    pid_t pid = fork();
    if (pid == 0) {
        pthread_mutex_lock(&glock);
        sleep(1);
        pthread_mutex_unlock(&glock);
        if (gchild_flag == 1) {
            printf("gchild_flag verify success\n");
        } else {
            printf("gchild_flag verify fail: %d\n", gchild_flag);
        }
        _exit(0);
    } else if (pid > 0) {
        pthread_join(tid, NULL);

        if (gprepare_flag == 1) {
            printf("gprepare_flag verify success\n");
        } else {
            printf("gprepare_flag verify fail: %d\n", gprepare_flag);
        }

        if (gparent_flag == 1) {
            printf("gparent_flag verify success\n");
        } else {
            printf("gparent_flag verify fail: %d\n", gparent_flag);
        }
    } else {
        printf("fork failed\n");
        return -1;
    }

    pthread_mutex_destroy(&sync_lock);
    pthread_cond_destroy(&sync_cond);
    pthread_mutex_destroy(&glock);
    return 0;
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).