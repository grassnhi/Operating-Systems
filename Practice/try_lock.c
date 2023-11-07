#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *arg) {
    int result;
    while (1) {
        result = pthread_mutex_trylock(&mutex);
        if (result == 0) {
            printf("Thread %d acquired lock.\n", (int)(size_t)arg);
            sleep(2);
            pthread_mutex_unlock(&mutex);
            break;
        } else {
            printf("Thread %d failed to acquire lock. Sleeping...\n", (int)(size_t)arg);
            sleep(1);
        }
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread_function, (void *)1);
    pthread_create(&thread2, NULL, thread_function, (void *)2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
