#include <stdio.h>
#include <pthread.h>
#include "seqlock.h"

// rest of the code


// Initialize the sequence lock
int pthread_seqlock_init(pthread_seqlock_t *seqlock) {
    // Initialize the mutex and condition variables
    pthread_mutex_init(&seqlock->mutex, NULL);
    pthread_cond_init(&seqlock->cond, NULL);
    // Set the initial sequence to 0
    seqlock->sequence = 0;
    return 0;
}

// Destroy the sequence lock
int pthread_seqlock_destroy(pthread_seqlock_t *seqlock) {
    // Destroy the mutex and condition variables
    pthread_mutex_destroy(&seqlock->mutex);
    pthread_cond_destroy(&seqlock->cond);
    return 0;
}

// Acquire a read lock
int pthread_seqlock_rdlock(pthread_seqlock_t *seqlock) {
    // Lock the mutex to read the current sequence
    pthread_mutex_lock(&seqlock->mutex);
    int current_sequence = seqlock->sequence;
    pthread_mutex_unlock(&seqlock->mutex);

    // Wait for a writer if the sequence is odd
    while (current_sequence % 2 != 0) {
        pthread_mutex_lock(&seqlock->mutex);
        pthread_cond_wait(&seqlock->cond, &seqlock->mutex);
        pthread_mutex_unlock(&seqlock->mutex);
    }
    return 0;
}

// Release a read lock
int pthread_seqlock_rdunlock(pthread_seqlock_t *seqlock) {
    // No need for explicit unlock in the reader case, return 0
    return 0;
}

// Acquire a write lock
int pthread_seqlock_wrlock(pthread_seqlock_t *seqlock) {
    // Lock the mutex to wait for ongoing reads to finish
    pthread_mutex_lock(&seqlock->mutex);
    // Wait for readers to finish if any
    while (seqlock->sequence % 2 != 0) {
        pthread_cond_wait(&seqlock->cond, &seqlock->mutex);
    }
    // Increment the sequence to indicate a writer is active
    seqlock->sequence++;
    return 0;
}

// Release a write lock
int pthread_seqlock_wrunlock(pthread_seqlock_t *seqlock) {
    // Release the lock and notify waiting threads
    seqlock->sequence++;
    pthread_cond_broadcast(&seqlock->cond);
    pthread_mutex_unlock(&seqlock->mutex);
    return 0;
}

pthread_seqlock_t lock;

#define NUM_READERS 3
#define NUM_WRITERS 2


void *reader(void *arg) {
    pthread_seqlock_rdlock(&lock);
    printf("Reading data...\n");
    pthread_seqlock_rdunlock(&lock);
    return NULL;
}

void *writer(void *arg) {
    pthread_seqlock_wrlock(&lock);
    printf("Writing data...\n");
    pthread_seqlock_wrunlock(&lock);
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    pthread_seqlock_init(&lock);

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_create(&readers[i], NULL, reader, NULL);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_seqlock_destroy(&lock);

    return 0;
}