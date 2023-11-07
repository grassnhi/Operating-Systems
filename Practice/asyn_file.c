#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_FILES 10
#define MAX_CONCURRENT 3

sem_t semaphore;

void* download_file(void* arg) {
    // Simulating file downloading
    int file_number = *((int*)arg);
    printf("Downloading file %d...\n", file_number);
    // Simulating file processing time
    sleep(2);
    printf("File %d downloaded.\n", file_number);
    sem_post(&semaphore);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[MAX_FILES];
    int file_numbers[MAX_FILES];
    sem_init(&semaphore, 0, MAX_CONCURRENT);

    for (int i = 0; i < MAX_FILES; i++) {
        file_numbers[i] = i;
        sem_wait(&semaphore);
        pthread_create(&threads[i], NULL, download_file, &file_numbers[i]);
    }

    for (int i = 0; i < MAX_FILES; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);

    return 0;
}
