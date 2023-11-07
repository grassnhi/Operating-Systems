#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

pthread_mutex_t mtx;
pthread_cond_t chopstick[N];
enum States {THINKING, HUNGRY, EATING};
enum States state[N] = {THINKING};

void test(int);
void *philosopher(void*);
void eat(int);
void think(int);

int main(){
    int i, a[N];
    pthread_t tid[N];

    pthread_mutex_init(&mtx, NULL);
    for(i = 0; i < N; i++)
        pthread_cond_init(&chopstick[i], NULL);

    alarm(10);

    for(i = 0; i < N; i++){
        a[i] = i;
        pthread_create(&tid[i], NULL, philosopher, (void *) &a[i]);
    }

    for(i = 0; i < N; i++)
        pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&mtx);
    return 0;
}

void test(int phil){
    if((state[(phil + 4) % N] != EATING) && (state[phil] == HUNGRY)
    && (state[(phil + 1) % N] != EATING)){
        state[phil] = EATING;
        pthread_cond_signal(&chopstick[phil]);
    }
}

void *philosopher(void *num){
    int phil = *(int*) num;
    printf("Philosopher %d has entered room\n", phil);
    sleep(1);
    while(1){
        pthread_mutex_lock(&mtx);

        state[phil] = HUNGRY;
        test(phil);
        if(state[phil] != EATING)
            pthread_cond_wait(&chopstick[phil], &mtx);

        printf("Philosopher %d takes fork %d and %d\n", phil, phil, (phil + 1) % N);

        eat(phil);
        sleep(2);

        state[phil] = THINKING;
        test((phil + 4) % N);
        test((phil + 1) % N);

        printf("Philosopher %d puts fork %d and %d down\n", phil, (phil + 1) % N, phil);

        think(phil);
        pthread_mutex_unlock(&mtx);
        sleep(2);
    }
}

void eat(int phil){
    printf("Philosopher %d is eating\n", phil);
    state[phil] = EATING;
}

void think(int phil){
    printf("Philosopher %d is thinking\n", phil);
}
