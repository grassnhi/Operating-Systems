#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define FILESIZE 100

int main() {
    const char *memname = "shared_memory.txt";
    const char *message = "Hello, shared memory!";
    int fd;
    char *map;

    // Create and open a shared memory area
    fd = shm_open(memname, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Error opening file for writing");
        exit(1);
    }

    // Configure the size of the shared memory segment
    ftruncate(fd, strlen(message) + 1);

    // Map the shared memory segment to the local address space
    map = mmap(0, strlen(message) + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        perror("mmap");
        exit(1);
    }

    // Write a message to the shared memory
    sprintf(map, "%s", message);

    // Create a child process
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    } else if (pid == 0) { // Child process
        printf("Child process reading from shared memory: %s\n", map);
    } else { // Parent process
        wait(NULL);
        printf("Parent process reading from shared memory: %s\n", map);
    }

    // Remove the shared memory segment
    if (shm_unlink(memname) == -1) {
        perror("Error removing shared memory");
        exit(-1);
    }

    return 0;
}
