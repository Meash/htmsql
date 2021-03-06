#include "atomic_exch_hle_lock-busy.h"
#include <xmmintrin.h> // _mm_pause

void atomic_exch_hle_lock_busy(type *lock) {
	while (__atomic_exchange_n(lock, 1,
			__ATOMIC_ACQUIRE | __ATOMIC_HLE_ACQUIRE))
		_mm_pause(); /* Abort failed transaction */
}
void atomic_exch_hle_unlock_busy(type *lock) {
	/* Free lock with lock elision */
	__atomic_clear(lock, __ATOMIC_RELEASE | __ATOMIC_HLE_RELEASE);
//	__atomic_store_n(lock, 0, __ATOMIC_RELEASE | __ATOMIC_HLE_RELEASE);
	// http://gcc.gnu.org/onlinedocs/gcc/x86-specific-memory-model-extensions-for-transactional-memory.html
}
