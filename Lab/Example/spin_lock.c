#include <pthread.h>

pthread_spinlock_t lock;
int pshared;

// Initialize the spin lock
int pthread_spin_init(pthread_spinlock_t *lock, int pshared);

// Destroy the spin lock
int pthread_spin_destroy(pthread_spinlock_t *lock);

// Lock the spin lock
int pthread_spin_lock(pthread_spinlock_t *lock);

// Unlock the spin lock
int pthread_spin_unlock(pthread_spinlock_t *lock);
