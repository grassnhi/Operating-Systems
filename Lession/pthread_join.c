#define NUM THREADS 10 
/* an array of threads to be joined upon */ 
pthread_t workers[NUM THREADS]; 

for (int i = 0; i < NUM THREADS; i++) 
    pthread_join(workers[i], NULL);
