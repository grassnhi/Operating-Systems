#include<pthread.h>
#include<stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *PrintHello(void *threadid){
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me thread %ld!\n", tid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for(t = 0; t < NUM_THREADS; t++){
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if(rc){
            printf("ERROR; return from pthread create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}

//gcc -o thread thread.c -lpthread

/*

struct thread_data{
    int thread_id;
    int sum;
    char *message;
}

struct thread_data thread_data_array[NUM_THREADS];

void *PrintHello(void *thread_arg){
    struct thread_data *my_data;
    ...
    my_data = (struct thread_data *) thread_arg;
    taskid = my_data->thread_id;
    sum = my_data->sum;
    hello_msg = my_data->message;
    ...
}

int main(int argc, char *argv[]){
    ...
    thread_data_array[t].thread_id = t;
    thread_data_array[t].sum = sum;
    thread_data_array[t].message = messages[t];
    rc = pthread_create(&threads[t], NULL, PrintHello, (void *)&thread_data_array[t]);
    ...
}

*/