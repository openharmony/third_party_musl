#include <sched.h>
#include "pthread_impl.h"

int pthread_yield(void) {
    return sched_yield();
}