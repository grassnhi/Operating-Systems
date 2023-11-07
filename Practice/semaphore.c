#include <semaphore.h>

sem_t sem;

// Initialize the semaphore
int sem_init(sem_t *sem, int pshared, unsigned int value);

// Destroy the semaphore
int sem_destroy(sem_t *sem);

// Wait on the semaphore
int sem_wait(sem_t *sem);

// Post to the semaphore
int sem_post(sem_t *sem);
