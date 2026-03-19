#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "test.h"

void check_load_by_prelink(const char *keyword);

void set_prelink_chk_enable(void);

void do_dlopen(const char *name, int mode);

void dlopen_lazy();

void dlopen_now();

void dlopen_global();

void dlopen_local();

void dlopen_so_used_by_dlsym();

void dlopen_nodelete_and_noload();

void dlopen_dlclose();

void dlopen_dlclose_weak();

void dlclose_recursive();

void *dlclose_recursive_thread();

void dlclose_recursive_by_multipthread();

void dlopen_global_test();

void dlclose_exit_test();

void dlclose_with_tls_test();
