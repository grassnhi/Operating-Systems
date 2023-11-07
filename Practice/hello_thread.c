#include <stdio.h>
#include <pthread.h>

#define MAXCOUNT 10000
int count;

void *fcount(void *sid) {
    int i;
    for (i = 0; i < MAXCOUNT; i++) {
        count = count + 1;
    }
    printf("Thread %s: holding %d\n", (char *)sid, count);
    getc(stdin);
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("Hello world\n");
    pthread_t thread1, thread2;
    count = 0;

    /* Create independent threads each of which will execute function */
    pthread_create(&thread1, NULL, fcount, "1");
    pthread_create(&thread2, NULL, fcount, "2");

    // Wait for thread th1 finish
    pthread_join(thread1, NULL);
    // Wait for thread th2 finish
    pthread_join(thread2, NULL);
    getc(stdin);
    return 0;
}
