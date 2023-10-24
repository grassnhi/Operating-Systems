#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define MAX_MOVIES 1682
#define MAX_LINE_LENGTH 100

int shmid;

void calculateAverage(double (*ratings)[MAX_MOVIES], char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        int userID, movieID, rating;
        long timeStamp;
        if (sscanf(line, "%d\t%d\t%d\t%ld", &userID, &movieID, &rating, &timeStamp) == 4) {
            if (movieID > 0 && movieID <= MAX_MOVIES) {
                ratings[0][movieID-1] += rating; // rate
                ratings[1][movieID-1] += 1; // user
            } else {
                fprintf(stderr, "Invalid movie ID: %d\n", movieID);
            }
        } else {
            fprintf(stderr, "Error parsing line: %s\n", line);
        }
    }

    fclose(file);
}

int main() {
    key_t key = IPC_PRIVATE;
    double (*ratings)[MAX_MOVIES];

    if ((shmid = shmget(key, sizeof(double) * 2 * MAX_MOVIES, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((ratings = shmat(shmid, NULL, 0)) == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    for (int j = 0; j < MAX_MOVIES; j++) {
        ratings[0][j] = 0.0;
        ratings[1][j] = 0.0;
    }

    pid_t child1, child2;
    child1 = fork();

    if (child1 == 0) {
        calculateAverage(ratings, "movie-100k_1.txt");
        shmdt(ratings);
        exit(0);
    }

    child2 = fork();

    if (child2 == 0) {
        calculateAverage(ratings, "movie-100k_2.txt");
        shmdt(ratings);
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    for (int j = 0; j < MAX_MOVIES; j++) {
        if (ratings[1][j] > 0) {
            printf("Movie ID = %d, Number of Users = %d, Average Rating = %f\n", j+1, (int)ratings[1][j], ratings[0][j] / ratings[1][j]);
        }
    }

    if (shmdt(ratings) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}
