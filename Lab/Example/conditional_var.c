#include <pthread.h>

pthread_mutex_t mtx;
pthread_cond_t lock;

// Initialize the mutex and conditional variable
pthread_mutex_init(&mtx, NULL);
pthread_cond_init(&lock, NULL);

// Wait on the conditional variable
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

// Signal the conditional variable
int pthread_cond_signal(pthread_cond_t *cond);
