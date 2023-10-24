#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 100

typedef struct {
    int start;
    int end;
    long long sum;
} ThreadData;

void *partial_sum(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->sum = 0;
    for (int i = data->start; i <= data->end; i++) {
        data->sum += i;
    }
    return NULL;
}

long long sum_multi_thread(int n, int numThreads) {
    pthread_t threads[MAX_THREADS];
    ThreadData threadData[MAX_THREADS];
    int section = n / numThreads;
    long long totalSum = 0;

    for (int i = 0; i < numThreads; i++) {
        threadData[i].start = i * section + 1;
        threadData[i].end = (i + 1) * section;
        pthread_create(&threads[i], NULL, partial_sum, &threadData[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        totalSum += threadData[i].sum;
    }

    return totalSum;
}

int main(int argc, char *argv[]) {
    long numThreads = strtol(argv[1], NULL, 10);
	long n = strtol(argv[2], NULL, 10);
    long long result = sum_multi_thread(n, numThreads);
    printf("Sum of first %ld numbers using %ld threads is %lld (multi-thread)\n", n, numThreads, result);

    return 0;
}
