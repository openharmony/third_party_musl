#include "pthread_impl.h"

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	/* If the mutex being destroyed is process-shared and has nontrivial
	 * type (tracking ownership), it might be in the pending slot of a
	 * robust_list; wait for quiescence. */
	int type = mutex->_m_type;
	if (type > 128) __vm_wait();
	if (__is_mutex_destroyed(type))
		__handle_using_destroyed_mutex(__FUNCTION__);
	return 0;
}
