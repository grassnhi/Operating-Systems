#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define MAXITEMS 1
#define THREADS 1 // 1 producer and 1 consumer
#define LOOPS 2 * MAXITEMS // variable

// Initiate shared buffer
int buffer[MAXITEMS];
int fill = 0;
int use = 0;
sem_t empty, full;

// Function prototypes
void put(int value); // put data into buffer
int get(); // get data from buffer

// Producer function
void *producer(void *arg) {
    int i;
    int tid = *(int*)arg;
    for (i = 0; i < LOOPS; i++) {
        sem_wait(&empty);
        put(i); // line P2
        printf("Producer %d put data %d\n", tid, i);
        sem_post(&full);
        sleep(1);
    }
    pthread_exit(NULL);
}

// Consumer function
void *consumer(void *arg) {
    int i, tmp = 0;
    int tid = *(int*)arg;
    while (tmp != -1) {
        sem_wait(&full);
        tmp = get(); // line C2
        printf("Consumer %d get data %d\n", tid, tmp);
        sem_post(&empty);
        sleep(1);
    }
    pthread_exit(NULL);
}

// Main function
int main(int argc, char **argv) {
    int i, j;
    int tid[THREADS];
    pthread_t producers[THREADS];
    pthread_t consumers[THREADS];

    sem_init(&empty, 0, MAXITEMS);
    sem_init(&full, 0, 0);

    for (i = 0; i < THREADS; i++) {
        tid[i] = i;
        // Create producer thread
        pthread_create(&producers[i], NULL, producer, (void *)&tid[i]);
        // Create consumer thread
        pthread_create(&consumers[i], NULL, consumer, (void *)&tid[i]);
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}

// Put function
void put(int value) {
    buffer[fill] = value; // line f1
    fill = (fill + 1) % MAXITEMS; // line f2
}

// Get function
int get() {
    int tmp = buffer[use]; // line g1
    use = (use + 1) % MAXITEMS; // line g2
    return tmp;
}
