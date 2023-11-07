#include <pthread.h>

pthread_rwlock_t lock;

// Initialize the reader-writer lock
int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr);

// Destroy the reader-writer lock
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

// Acquire the reader lock
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);

// Acquire the writer lock
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);

// Unlock the reader-writer lock
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
