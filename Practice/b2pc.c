#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_ITEMS 3
#define THREADS 1 // 1 producer and 1 consumer
#define LOOPS MAX_ITEMS*2 // variable

// Initiate shared buffer
int buffer[MAX_ITEMS];
int fill = 0;
int use = 0;

// Semaphore
sem_t mutex, full, empty;

void put(int value); // put data into buffer
int get(); // get data from buffer

void * producer(void * arg){
	int i;
	int tid = *(int*) arg;
	for(i = 0; i < LOOPS; i++){
		sem_wait(&empty);
		sem_wait(&mutex);
		put(i); // line P2
		printf("Producer %d put data %d\n", tid, i);
		sem_post(&mutex);
		sem_post(&full);
		sleep(1);
	}
	pthread_exit(NULL);
}

void * consumer(void * arg){
	int i, tmp;
	int tid = *(int*) arg;
	for(i = 0; i < LOOPS; i++){
		sem_wait(&full);
		sem_wait(&mutex);
		tmp = get(); // line C2
		printf("Consumer %d get data %d\n", tid, tmp);
		sem_post(&mutex);
		sem_post(&empty);
		sleep(1);
	}
	pthread_exit(NULL);
}

int main(int argc, char ** argv){
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, MAX_ITEMS);
	int i;
	long int tid[THREADS];
	pthread_t producers[THREADS];
	pthread_t consumers[THREADS];

	for(i = 0; i < THREADS; i++){
		tid[i] = i;
		// Create producer thread
		pthread_create(&producers[i], NULL, producer, (void *) &tid[i]);

		// Create consumer thread
		pthread_create(&consumers[i], NULL, consumer, (void *) &tid[i]);
	}

	// Wait for the threads to finish
	for(i = 0; i < THREADS; i++){
		pthread_join(producers[i], NULL);
		pthread_join(consumers[i], NULL);
	}
	sem_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);
	return 0;
}

void put(int value){
	buffer[fill] = value; // line f1
	fill = (fill + 1) % MAX_ITEMS; // line f2
}

int get(){
	int tmp = buffer[use]; // line g1
	use = (use + 1) % MAX_ITEMS; // line g2
	return tmp;
}
