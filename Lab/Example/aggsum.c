#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_ARRAY_SIZE 100

int sumbuff = 0;
pthread_mutex_t mutex; // Global mutex for thread safety

struct range {
    int start;
    int end;
};

void generate_array_data(int* buf, int arrsz, int seednum) {
    // Code to generate array data
    srand(seednum);
    for (int i = 0; i < arrsz; i++) {
        buf[i] = rand() % 100; // Generate random integers between 0 and 99
    }
}

void* sum_worker(void *arg) {
    struct range *idx_range = (struct range *)arg;
    int partial_sum = 0;
    for (int i = idx_range->start; i <= idx_range->end; i++) {
        partial_sum += i; // Customize this line for specific array processing
    }
    pthread_mutex_lock(&mutex); // Lock the critical section
    sumbuff += partial_sum; // Update the sum in the critical section
    pthread_mutex_unlock(&mutex); // Unlock the critical section
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: aggsum arrsz tnum [seednum]\n");
        return 1;
    }
    int arrsz = atoi(argv[1]);
    int tnum = atoi(argv[2]);
    int seednum = argc > 3 ? atoi(argv[3]) : 0;

    int buf[MAX_ARRAY_SIZE];
    generate_array_data(buf, arrsz, seednum);

    pthread_t threads[tnum];
    struct range thread_idx_range;

    pthread_mutex_init(&mutex, NULL); // Initialize the global mutex

    int range_size = arrsz / tnum;
    int remainder = arrsz % tnum;
    int start = 0;
    int end = -1;
    for (int i = 0; i < tnum; i++) {
        start = end + 1;
        end = start + range_size - 1 + (i < remainder ? 1 : 0);
        thread_idx_range.start = start;
        thread_idx_range.end = end;
        pthread_create(&threads[i], NULL, sum_worker, (void *)&thread_idx_range);
    }

    for (int i = 0; i < tnum; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex); // Destroy the global mutex

    printf("Sum: %d\n", sumbuff);

    return 0;
}
