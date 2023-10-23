#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

struct thread_data {
    int thread_id;
    int sum;
    char *message;
};

struct thread_data thread_data_array[NUM_THREADS];

void *PrintHello(void *thread_arg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *)thread_arg;
    int taskid = my_data->thread_id;
    int sum = my_data->sum;
    char *hello_msg = my_data->message;

    printf("Hello World! It's me thread %d: %s Sum=%d\n", taskid, hello_msg, sum);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc;
    int t;
    char *messages[NUM_THREADS] = {
        "Msg 1:",
        "Msg 2:",
        "Msg 3:",
        "Msg 4:",
        "Msg 5:"
    };

    int sum = 0;
    for (t = 0; t < NUM_THREADS; t++) {
        sum += t;
        printf("In main: creating thread %d\n", t);

        thread_data_array[t].thread_id = t;
        thread_data_array[t].sum = sum;
        thread_data_array[t].message = messages[t];

        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)&thread_data_array[t]);
        if (rc) {
            printf("ERROR; return from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Wait for all threads to finish */
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("Main thread exiting.\n");
    pthread_exit(NULL);
}
