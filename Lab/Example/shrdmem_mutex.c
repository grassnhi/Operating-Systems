#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int MAXCOUNT = 1e9;
static int count = 0;
pthread_mutex_t mutex; // declare a mutex

void *fcount(void *sid) {
    int i;
    for (i = 0; i < MAXCOUNT; i++) {
        pthread_mutex_lock(&mutex); // lock the critical section
        count = count + 1;
        pthread_mutex_unlock(&mutex); // unlock the critical section
    }
    printf("Thread %s: holding %d \n", (char *)sid, count);
}

int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, NULL); // initialize the mutex

    pthread_create(&thread1, NULL, &fcount, "1");
    pthread_create(&thread2, NULL, &fcount, "2");

    // Wait for thread 1 and thread 2 to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex); // destroy the mutex
    return 0;
}
