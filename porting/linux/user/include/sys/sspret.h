#ifndef _SSPRET_H
#define _SSPRET_H

#if defined(__SSP_RET_STRONG__) || defined(__SSP_RET_ALL__)
#if defined(__aarch64__)
# define SSPRET_CALC_RETCOOKIE(reg) \
    eor     reg, reg, x30

# define SSPRET_LOAD_COOKIE(x, reg) \
    mov     reg, x29

# define SSPRET_SETUP(x, reg) \
    SSPRET_LOAD_COOKIE(x, reg); \
    SSPRET_CALC_RETCOOKIE(reg)

# define SSPRET_CHECK(x, reg) \
    SSPRET_LOAD_COOKIE(x, x9); \
    SSPRET_CALC_RETCOOKIE(x9); \
    subs    reg, reg, x9; \
    cbz     reg, 6788f; \
    brk     #0x1; \
6788:

# define SSPRET_PUSH(reg) \
    str     reg, [sp, #-16]!

# define SSPRET_POP(reg) \
    ldr     reg, [sp, #16]!

#endif /* __aarch64__ */
#else
# define SSPRET_CALC_RETCOOKIE(reg)
# define SSPRET_LOAD_COOKIE(x, reg)
# define SSPRET_SETUP(x, reg)
# define SSPRET_CHECK(x, reg)
# define SSPRET_PUSH(reg)
# define SSPRET_POP(reg)
#endif /* __SSP_RET_STRONG__ || __SSP_RET_ALL__ */

#endif /* _SSPRET_H */
