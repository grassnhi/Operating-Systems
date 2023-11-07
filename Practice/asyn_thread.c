#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

pthread_t threads[NUM_THREADS];

void *thread_function(void *arg) {
    int thread_num = *((int *)arg);
    printf("Thread %d is starting...\n", thread_num);
    sleep(3); // Simulating work
    printf("Thread %d is done.\n", thread_num);
    return NULL;
}

int main() {
    int i;
    int thread_args[NUM_THREADS];

    // Creating threads
    for (i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Joining threads
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
        printf("Thread %d has completed.\n", i);
    }

    return 0;
}
