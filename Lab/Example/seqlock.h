#ifndef SEQLOCK_H
#define SEQLOCK_H

#include <pthread.h>

// Define the pthread_seqlock structure
typedef struct pthread_seqlock {
    pthread_mutex_t mutex; // Mutex for ensuring mutual exclusion
    pthread_cond_t cond; // Condition variable for signaling and synchronization
    int sequence; // Sequence number for tracking the lock status
} pthread_seqlock_t;

// Initialize the sequence lock
int pthread_seqlock_init(pthread_seqlock_t *seqlock);

// Destroy the sequence lock
int pthread_seqlock_destroy(pthread_seqlock_t *seqlock);

// Acquire a read lock
int pthread_seqlock_rdlock(pthread_seqlock_t *seqlock);

// Release a read lock
int pthread_seqlock_rdunlock(pthread_seqlock_t *seqlock);

// Acquire a write lock
int pthread_seqlock_wrlock(pthread_seqlock_t *seqlock);

// Release a write lock
int pthread_seqlock_wrunlock(pthread_seqlock_t *seqlock);

#endif /* SEQLOCK_H */
