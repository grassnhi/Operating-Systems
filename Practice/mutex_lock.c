#include <pthread.h>

pthread_mutex_t lock;

// Initialize the mutex
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

// Destroy the mutex
int pthread_mutex_destroy(pthread_mutex_t *mutex);

// Lock the mutex
int pthread_mutex_lock(pthread_mutex_t *mutex);

// Unlock the mutex
int pthread_mutex_unlock(pthread_mutex_t *mutex);
