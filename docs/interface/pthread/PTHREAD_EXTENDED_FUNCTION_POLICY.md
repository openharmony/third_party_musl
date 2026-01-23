### **PTHREAD_EXTENDED_FUNCTION_POLICY        OHOS Programmer's Manual**


#### **NAME**

​       set_pthread_extended_function_policy - set whether to enable the flag for controlling thread affinity series interfaces at runtime.

​       get_pthread_extended_function_policy - get the flag indicating whether the control thread affinity series interfaces are enabled at runtime.


#### **SYNOPSIS**

       #define _GNU_SOURCE
       #include <pthread.h>

       void set_pthread_extended_function_policy(int flag);

       int get_pthread_extended_function_policy(void);


#### **DESCRIPTION**

​       When the parameter flag of the set_pthread_extended_function_policy() function is non-zero, it indicates that the thread affinity series interfaces are enabled. On the contrary, it means cancellation. After enabling this tag setting, please make sure to disable it promptly when not in use.

#### **RETURN VALUE**

​       set_pthread_extended_function_policy() has no return value.
​       get_pthread_extended_function_policy() returns 0 or non-zero.

#### ATTRIBUTES

| Interface                                                    | Attribute     | Value    |
| ------------------------------------------------------------ | ------------- | -------- |
| set_pthread_extended_function_policy(), <br> get_pthread_extended_function_policy()| Thread safety | MT-unsafe  |
|                                                              | Signal safety | Not Safe |


#### HISTORY

​       -- 2026

#### NOTES

​      These feature are designed specifically for when musl_extended_function is true.

#### EXAMPLES

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#ifdef MUSL_EXTERNAL_FUNCTION
#ifndef __LITEOS_A__

int main(void) {
    int current_policy;
    int test_passed = 1;

    current_policy = get_pthread_extended_function_policy();
    if (current_policy != 0) {
        printf("Initial policy should be 0, actually got %d\n", current_policy);
        test_passed = 0;
    }

    set_pthread_extended_function_policy(1); // non-zero
    current_policy = get_pthread_extended_function_policy();
    if (current_policy != 1) {
        printf("Policy should be 1 after set_pthread_extended_function_policy(1), actually got %d\n", current_policy);
        test_passed = 0;
    }

    if (current_policy != 0) {
        pthread_attr_t attr;
        cpu_set_t cpuset;

        if (pthread_attr_init(&attr) != 0) {
            printf("Failed to initialize thread attributes\n");
            test_passed = 0;
        } else {
            CPU_ZERO(&cpuset);
            CPU_SET(0, &cpuset);

            int ret = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
            if (ret != 0) {
                printf("Failed to set CPU affinity (error code: %d)\n", ret);
                test_passed = 0;
            }

            pthread_attr_destroy(&attr);
        }
    }

    set_pthread_extended_function_policy(0);
    current_policy = get_pthread_extended_function_policy();
    if (current_policy != 0) {
        printf("Policy should be 0 after set_pthread_extended_function_policy(0), actually got %d\n", current_policy);
        test_passed = 0;
    }

    return test_passed ? 0 : 1;
}
#endif
#endif
```


#### COLOPHON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
