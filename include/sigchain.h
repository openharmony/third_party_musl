#ifndef _SIGCHAIN_H
#define _SIGCHAIN_H

#ifdef __cplusplus
extern "C" {
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <signal.h>
#include <stdint.h>
#include <stdbool.h>

static const int SIGCHAIN_ALLOW_NORETURN = 0x1UL;

/* The action of the sigchain. */
struct signal_chain_action {
  bool (*sca_sigaction)(int, siginfo_t*, void*);
  sigset_t sca_mask;
  int sca_flags;
};

/* Mark the signal to the sigchain, add the special handler to the sigchain. */
void add_special_signal_handler(int signo, struct signal_chain_action* sa);
/* Remove the special the handler form the sigchain. */
void remove_special_signal_handler(int signo, bool (*fn)(int, siginfo_t*, void*));


#ifdef __cplusplus
}
#endif

#endif