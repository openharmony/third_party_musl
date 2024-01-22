#include <stdio.h>
#include <syscall.h>
#include <assert.h>
#include <syscall_hooks.h>

#include "libc.h"
#include "pthread_impl.h"

#define HIJACK_SYSCALL_MAX  1024

volatile const char *g_syscall_hooks_table __attribute__((aligned(8))) = NULL;
volatile void *g_syscall_hooks_entry __attribute__((aligned(8))) = NULL;

/*
 * @brief
 *     reset == 0: g_syscall_hooks_table and g_syscall_hooks_entry will be the function
 *                 input parameters hooks_table and hooks_entry.
 *     reset != 0: g_syscall_hooks_table and g_syscall_hooks_entry will be default values
 *
 * @param
 *     hooks_table: pointer to a syscall table
 *     hooks_entry: pointer to svc0_entry
 *
 * @return
 *     -EINVAL: input parameters are invalid
 *     -EPERM: operation not allowed
 *     0: set_syscall_hooks success   
*/
extern volatile int __thread_list_lock;
int set_syscall_hooks(const char *hooks_table, int table_len, void *hooks_entry, int reset, int **tid_addr)
{
    int ret = 0;

    if (reset == 0) {
        if (hooks_table == NULL || hooks_entry == NULL || tid_addr == NULL ||
            table_len < HIJACK_SYSCALL_MAX) {
                ret = -EINVAL;
            }
    }
    
    if (ret == 0) {
        __tl_lock();
        if (!libc.threads_minus_1) {
            if (reset) {
                g_syscall_hooks_entry = NULL;
                g_syscall_hooks_table = NULL;
            } else {
                g_syscall_hooks_entry = hooks_entry;
                g_syscall_hooks_table = hooks_table;
            }
            *tid_addr = &__thread_list_lock;
        } else {
            ret = -EPERM;
        }
        __tl_unlock();
    }

    return ret;
}