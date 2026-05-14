#include <setjmp.h>
#include <signal.h>
#include "syscall.h"
#include "pthread_impl.h"
#include <unsupported_api.h>

_Noreturn void siglongjmp(sigjmp_buf buf, int ret)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	/* If sigsetjmp was called with nonzero savemask flag, the address
	 * longjmp will return to is inside of sigsetjmp. The signal mask
	 * will then be restored in the returned-to context instead of here,
	 * which matters if the context we are returning from may not have
	 * sufficient stack space for signal delivery. */
	longjmp(buf, ret);
}
