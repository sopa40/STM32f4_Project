/**
 * libsk syncronization primitives
 */

#include <stdbool.h>
#include <stdint.h>


/** 
 * Type declaration for lock primitives.
 * 0 means unlocked, 1 means locked -- to create locks unlocked by default
 *
 * Locks are 8-bit variables, access to which should always be atomic.
 * Under hood, atomic access is achieved using STREXB and LDREXB instructions.
 * For user, the higher-level lock methods are implemented.
 */
typedef uint8_t sk_lock_t;

#define __SK_LOCK_UNLOCKED ((sk_lock_t)0)
#define __SK_LOCK_LOCKED   ((sk_lock_t)(! (__SK_LOCK_UNLOCKED)))
// Mimic linux/spinlock.h Kernel interface DEFINE_SPINLOCK()
#define SK_LOCK_DECLARE(name) sk_lock_t name = __SK_LOCK_UNLOCKED


/**
 * Unlock a lock
 * @lock: pointer to :c:type:`sk_lock_t` object
 *
 * Note: Unlock does not control who captured the lock and whether is was captured at all,
 *       it simply unlocks
 */
void sk_lock_unlock(sk_lock_t *lock);


/**
 * Try to capture a lock (non-blocking)
 * @lock: pointer to :c:type:`sk_lock_t` object
 * @return: `true` if lock was succesfully captured, `false` otherwise
 *
 * This call IS NOT blocking. It instantly returns status of lock capture procedure
 */
bool sk_lock_trylock(sk_lock_t *lock);


/**
 * Spin trying to capture a lock (blocking)
 * @lock: pointer to :c:type:`sk_lock_t` object
 *
 * This call IS blocking. It will return only after the lock was captured.
 * Thus deadlocks have more chances to occur
 */
void sk_lock_spinlock(sk_lock_t *lock);
