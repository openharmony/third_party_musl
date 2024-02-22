#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <info/fatal_message.h>
#define ASSERT_FATAL_MESSAGE_SIZE 1024

_Noreturn void __assert_fail(const char *expr, const char *file, int line, const char *func)
{
    // Concatenate information for assert failures and save the final result in assert_fatal_message
    char assert_fatal_message[ASSERT_FATAL_MESSAGE_SIZE];

    snprintf(assert_fatal_message, sizeof(assert_fatal_message),
             "Assertion failed: %s (%s: %s: %d)", expr, file, func, line);

    // call set_fatal_message to set assert_fatal_message
    set_fatal_message(assert_fatal_message);
    fprintf(stderr, "%s\n", assert_fatal_message);
    abort();
}
