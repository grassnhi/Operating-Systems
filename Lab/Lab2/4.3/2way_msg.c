#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MSG_SIZE 1024

struct message {
    long message_type;
    char text[MAX_MSG_SIZE];
};

int message_queue_id_1, message_queue_id_2;
pthread_mutex_t lock;
int can_send = 1;
int exit_program = 0;

void *sender(void *arg) {
    struct message msg;
    msg.message_type = 1;

    while (1) {
        pthread_mutex_lock(&lock);
        if (can_send) {
            printf("Enter a message to send to thread 2 (type 'exit' to quit): ");
            fgets(msg.text, MAX_MSG_SIZE, stdin);
            if (strcmp(msg.text, "exit\n") == 0) {
                exit_program = 1;
                pthread_mutex_unlock(&lock);
                break;
            }
            msgsnd(message_queue_id_2, &msg, sizeof(msg.text), 0);
            can_send = 0;
        }
        pthread_mutex_unlock(&lock);
        if (exit_program) {
            break;
        }
    }

    return NULL;
}


void *receiver(void *arg) {
    struct message msg;

    while (!exit_program) {
        msgrcv(message_queue_id_1, &msg, sizeof(msg.text), 1, 0);
        printf("Received message from thread 1: %s", msg.text);
        pthread_mutex_lock(&lock);
        can_send = 1;
        pthread_mutex_unlock(&lock);
        if (strcmp(msg.text, "exit\n") == 0) {
            exit_program = 1;
            break;
        }
    }

    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    // Creating two message queues
    key_t key1 = ftok("keyfile1", 65);
    key_t key2 = ftok("keyfile2", 66);

    message_queue_id_1 = msgget(key1, 0666 | IPC_CREAT);
    message_queue_id_2 = msgget(key2, 0666 | IPC_CREAT);

    if (message_queue_id_1 == -1 || message_queue_id_2 == -1) {
        perror("msgget");
        exit(1);
    }

    pthread_mutex_init(&lock, NULL);

    if (pthread_create(&tid1, NULL, sender, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&tid2, NULL, receiver, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Remove the message queues
    msgctl(message_queue_id_1, IPC_RMID, NULL);
    msgctl(message_queue_id_2, IPC_RMID, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}
