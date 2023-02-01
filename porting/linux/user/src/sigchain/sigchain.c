/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sigchain.h>
#include <locale.h>
#include <pthread.h>
#include <errno.h>
#include <threads.h>
#include <hilog_adapter.h>

extern int __libc_sigaction(int sig, const struct sigaction *restrict sa,
                            struct sigaction *restrict old);

#define SIG_CHAIN_KEY_VALUE_1 1
#define SIGNAL_CHAIN_SPECIAL_ACTION_MAX 2

#define SIGCHAIN_LOG_DOMAIN 0xD003F00
#define SIGCHAIN_LOG_TAG "SIGCHAIN"

#if (defined(OHOS_ENABLE_PARAMETER) || defined(ENABLE_MUSL_LOG))
#define SIGCHAIN_PRINT_ERROR(...) ((void)HiLogAdapterPrint(LOG_CORE, LOG_ERROR, SIGCHAIN_LOG_DOMAIN, SIGCHAIN_LOG_TAG, __VA_ARGS__))
#define SIGCHAIN_PRINT_INFO(...) ((void)HiLogAdapterPrint(LOG_CORE, LOG_INFO, SIGCHAIN_LOG_DOMAIN, SIGCHAIN_LOG_TAG, __VA_ARGS__))
#define SIGCHAIN_PRINT_DEBUG(...) ((void)HiLogAdapterPrint(LOG_CORE, LOG_DEBUG, SIGCHAIN_LOG_DOMAIN, SIGCHAIN_LOG_TAG, __VA_ARGS__))
#else
#define SIGCHAIN_PRINT_ERROR(...)
#define SIGCHAIN_PRINT_INFO(...)
#define SIGCHAIN_PRINT_DEBUG(...)
#endif

struct sc_signal_chain {
    bool marked;
    struct sigaction sig_action;
    struct signal_chain_action sca_special_actions[SIGNAL_CHAIN_SPECIAL_ACTION_MAX];
};

/* Signal chain set, from 0 to 63. */
static struct sc_signal_chain sig_chains[_NSIG - 1];
/* static thread Keyword */
static pthread_key_t g_sigchain_key;
/* This is once flag! */
static once_flag g_flag = ONCE_FLAG_INIT;

/**
  * @brief Create the thread key
  * @retval void
  */
void create_pthread_key(void)
{
    SIGCHAIN_PRINT_INFO("%{public}s create the thread key!", __func__);
    int rc = pthread_key_create(&g_sigchain_key, NULL);
    if (rc != 0) {
        SIGCHAIN_PRINT_ERROR("%{public}s failed to create sigchain pthread key, rc:%{public}d",
                __func__,  rc);
    }
}


/**
  * @brief Get the key of the signal thread.
  * @retval int32_t, the value of the sigchain key.
  */
static pthread_key_t get_handling_signal_key() {
    call_once(&g_flag, create_pthread_key);
    return g_sigchain_key;
}

/**
  * @brief Get the value of the sigchain key
  * @retval bool, true if set the value of the key，or false.
  */
static bool get_handling_signal() {
    void *result = pthread_getspecific(get_handling_signal_key());
    if (result == NULL) {
        return false;
    } else {
        return true;
    }
}

/**
  * @brief Set the value of the sigchain key
  * @param[in] value, the value of the sigchain key
  * @retval void.
  */
static void set_handling_signal(bool value)
{
    pthread_setspecific(get_handling_signal_key(),
                        (void *)((uintptr_t)(value)));
}

/**
  * @brief Judge whether the signal is marked
  * @param[in] signo, the value of the signal.
  * @retval true if the signal is marked, or false.
  */
bool ismarked(int signo)
{
    return sig_chains[signo - 1].marked;
}

/**
  * @brief This is a callback function, which is registered to the kernel
  * @param[in] signo, the value of the signal.
  * @param[in] siginfo, the information of the signal.
  * @param[in] ucontext_raw, the context of the signal.
  * @retval void
  */
static void signal_chain_handler(int signo, siginfo_t* siginfo, void* ucontext_raw)
{
    SIGCHAIN_PRINT_DEBUG("%{public}s signo: %{public}d", __func__, signo);
    /* Try to call the special handlers first. */
    /* If one of them crashes, we'll reenter this handler and pass that crash onto the user handler. */
    if (!get_handling_signal()){
        for (int i = 0; i < SIGNAL_CHAIN_SPECIAL_ACTION_MAX; i++) {
            if (sig_chains[signo - 1].sca_special_actions[i].sca_sigaction == NULL) {
                break;
            }
            /* The native bridge signal handler might not return. */
            bool noreturn = (sig_chains[signo - 1].sca_special_actions[i].sca_flags &
                             SIGCHAIN_ALLOW_NORETURN);
            sigset_t previous_mask;
            pthread_sigmask(SIG_SETMASK, &sig_chains[signo - 1].sca_special_actions[i].sca_mask,
                            &previous_mask);

            bool previous_value =  get_handling_signal();
            if (!noreturn) {
                set_handling_signal(true);
            }

            if (sig_chains[signo - 1].sca_special_actions[i].sca_sigaction(signo,
                                                            siginfo, ucontext_raw)) {
                set_handling_signal(previous_value);
                return;
            }

            pthread_sigmask(SIG_SETMASK, &previous_mask, NULL);
            set_handling_signal(previous_value);
        }
    }

    int sa_flags = sig_chains[signo - 1].sig_action.sa_flags;
    ucontext_t* ucontext = (ucontext_t*)(ucontext_raw);

    sigset_t mask;
    sigorset(&mask, &ucontext->uc_sigmask, &sig_chains[signo - 1].sig_action.sa_mask);

    if (!(sa_flags & SA_NODEFER)) {
        sigaddset(&mask, signo);
    }

    pthread_sigmask(SIG_SETMASK, &mask, NULL);

    if ((sa_flags & SA_SIGINFO)) {
        sig_chains[signo - 1].sig_action.sa_sigaction(signo, siginfo, ucontext_raw);
    } else {
        if (sig_chains[signo - 1].sig_action.sa_handler == SIG_IGN) {
            return;
        } else if (sig_chains[signo - 1].sig_action.sa_handler == SIG_DFL) {
            SIGCHAIN_PRINT_INFO("%{public}s exiting due to SIG_DFL handler for signal: %{public}d",
                    __func__, signo);
        } else {
            sig_chains[signo - 1].sig_action.sa_handler(signo);
        }
    }

    return;
}

/**
  * @brief Register the signal chain with the kernel if needed
  * @param[in] signo, the value of the signal.
  * @retval void
  */
void sigchain_register(int signo)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    struct sigaction signal_action = {};
    sigfillset(&signal_action.sa_mask);

    signal_action.sa_sigaction = signal_chain_handler;
    signal_action.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
    __libc_sigaction(signo, &signal_action, &sig_chains[signo - 1].sig_action);
}

/**
  * @brief Mark the signal to the sigchain.
  * @param[in] signo, the value of the signal.
  * @retval void
  */
void mark_signal_to_sigchain(int signo)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    if (!sig_chains[signo - 1].marked) {
        sigchain_register(signo);
        sig_chains[signo - 1].marked = true;
    }
}

/**
  * @brief Set the action of the signal.
  * @param[in] signo, the value of the signal.
  * @param[in] new_sa, the new action of the signal.
  * @retval void
  */
void setaction(int signo, const struct sigaction *restrict new_sa)
{
    SIGCHAIN_PRINT_DEBUG("%{public}s signo: %{public}d", __func__, signo);
    sig_chains[signo - 1].sig_action = *new_sa;
}

/**
  * @brief Get the action of the signal.
  * @param[in] signo, the value of the signal.
  * @retval The current action of the signal
  */
struct sigaction getaction(int signo)
{
    SIGCHAIN_PRINT_DEBUG("%{public}s signo: %{public}d", __func__, signo);
    return sig_chains[signo - 1].sig_action;
}

/**
  * @brief Add the special handler to the sigchain.
  * @param[in] signo, the value of the signal.
  * @param[in] sa, the action with special handler.
  * @retval void
  */
void add_special_handler(int signo, struct signal_chain_action* sa)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    for (int i = 0; i < SIGNAL_CHAIN_SPECIAL_ACTION_MAX; i++) {
        if (sig_chains[signo - 1].sca_special_actions[i].sca_sigaction == NULL) {
            sig_chains[signo - 1].sca_special_actions[i] = *sa;
            SIGCHAIN_PRINT_INFO("%{public}s signo %{public}d is registered with special handler!", __func__, signo);
            return;
        }
    }
}

/**
  * @brief Remove the special handler from the sigchain.
  * @param[in] signo, the value of the signal.
  * @param[in] fn, the special handler of the signal.
  * @retval void
  */
void rm_special_handler(int signo, bool (*fn)(int, siginfo_t*, void*))
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    int len = SIGNAL_CHAIN_SPECIAL_ACTION_MAX;
    for (int i = 0; i < len; i++) {
        if (sig_chains[signo - 1].sca_special_actions[i].sca_sigaction == fn) {
            for (int j = i; j < len - 1; ++j) {
                sig_chains[signo - 1].sca_special_actions[j] = 
                        sig_chains[signo - 1].sca_special_actions[j + 1];
            }
            sig_chains[signo - 1].sca_special_actions[len - 1].sca_sigaction = NULL;
            return;
        }
    }

    SIGCHAIN_PRINT_INFO("%{public}s failed to find special handler!. signo: %{public}d",
            __func__, signo);
}

/**
  * @brief This is an external interface, 
  *        Mark the signal to sigchain ,add the special handler to the sigchain.
  * @param[in] signo, the value of the signal.
  * @param[in] sa, the action with special handler.
  * @retval void
  */
void add_special_signal_handler(int signo, struct signal_chain_action* sa)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    if (signo <= 0 || signo >= _NSIG) {
        SIGCHAIN_PRINT_ERROR("%{public}s Invalid signal %{public}d", __func__, signo);
        return;
    }

    // Set the special handler.
    add_special_handler(signo, sa);
    mark_signal_to_sigchain(signo);
}

/**
  * @brief This is an external interface, remove the special handler from the sigchain.
  * @param[in] signo, the value of the signal.
  * @param[in] fn, the special handler of the signal.
  * @retval void
  */
void remove_special_signal_handler(int signo, bool (*fn)(int, siginfo_t*, void*))
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    if (signo <= 0 || signo >= _NSIG) {
        SIGCHAIN_PRINT_ERROR("%{public}s Invalid signal %{public}d", __func__, signo);
        return;
    }
    // remove the special handler.
    rm_special_handler(signo, fn);
}

/**
  * @brief Intercept the signal and sigaction.
  * @param[in] signo, the value of the signal.
  * @param[in] sa, the new action with the signal handler.
  * @param[out] old, the old action with the signal handler.
  * @retval true if the signal if intercepted, or false.
  */
bool intercept_sigaction(int signo, const struct sigaction *restrict sa,
                         struct sigaction *restrict old)
{
    SIGCHAIN_PRINT_DEBUG("%{public}s signo: %{public}d", __func__, signo);
    if (signo <= 0 || signo >= _NSIG) {
        SIGCHAIN_PRINT_ERROR("%{public}s Invalid signal %{public}d", __func__, signo);
        return false;
    }

    if (ismarked(signo)) {
        struct sigaction saved_action = getaction(signo);

        if (sa != NULL) {
            setaction(signo, sa);
        }
        if (old != NULL) {
            *old = saved_action;
        }
        return true;
    }

    return false;
}

/**
  * @brief Intercept the sigprocmask.
  * @param[in] how, the value of the mask operation .
  * @param[out] set, the value of the sigset.
  * @retval void.
  */
void intercept_sigprocmask(int how, sigset_t *restrict set)
{
    SIGCHAIN_PRINT_DEBUG("%{public}s how: %{public}d", __func__, how);
    // Forward directly to the pthread_sigmask When this sigchain is inside a signal handler
    if (get_handling_signal()) {
        return;
    }

    sigset_t tmpset;
    if (set != NULL) {
        tmpset = *set;
        if (how == SIG_BLOCK || how == SIG_SETMASK) {
            for (int i = 1; i < _NSIG; ++i) {
                if (ismarked(i) && sigismember(&tmpset, i)) {
                    sigdelset(&tmpset, i);
                }
            }
        }
        *set = tmpset;
    }

    return;
}
