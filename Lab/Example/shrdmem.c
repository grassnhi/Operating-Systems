#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int MAXCOUNT = 1e9;
static int count = 0;

void *fcount(void *sid) {
    int i;
    for (i = 0; i < MAXCOUNT; i++) {
        count = count + 1;
    }
    printf("Thread %s: holding %d \n", (char *)sid, count);
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, &fcount, "1");
    pthread_create(&thread2, NULL, &fcount, "2");

    // Wait for thread 1 and thread 2 to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
