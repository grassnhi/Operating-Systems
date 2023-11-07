#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <linux/sched.h>

#define MAX_WORKER 5
#define STACK_SIZE 65536
#define INFO

unsigned int wrkid_tid[MAX_WORKER];

void *bkwrk_worker(void *arg) {
    // Worker logic here
    return NULL;
}

void create_worker_thread() {
    unsigned int i;

    for (i = 0; i < MAX_WORKER; i++) {
#ifdef WORKTHREAD
        void **child_stack = (void **)malloc(STACK_SIZE);
        unsigned int wrkid = i;
        pthread_t threadid;

        sigset_t set;
        int s;

        sigemptyset(&set);
        sigaddset(&set, SIGQUIT);
        sigaddset(&set, SIGUSR1);
        sigprocmask(SIG_BLOCK, &set, NULL);

        // Stack grows down - start at top
        void *stack_top = child_stack + STACK_SIZE;

        wrkid_tid[i] = clone(&bkwrk_worker, stack_top, CLONE_VM | CLONE_FILES, (void *)&i);

#ifdef INFO
        fprintf(stderr, "bkwrk_create_worker using clone() got worker %u\n", wrkid_tid[i]);
#endif

        usleep(100);
#else
        // Using pthread_create
        pthread_t thread;
        int status = pthread_create(&thread, NULL, &bkwrk_worker, (void *)&i);
        wrkid_tid[i] = thread;

#ifdef INFO
        fprintf(stderr, "bkwrk_create_worker using pthread_create() got worker %u\n", wrkid_tid[i]);
#endif

        usleep(100);
#endif
    }
}

void create_worker_process() {
    pid_t pid;
    int i;

    for (i = 0; i < MAX_WORKER; i++) {
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed\n");
            exit(1);
        } else if (pid == 0) {
            // Child process
            bkwrk_worker(NULL);
            exit(0);
        } else {
            wrkid_tid[i] = pid;
#ifdef INFO
            fprintf(stderr, "bkwrk_create_worker using fork() got worker %u\n", wrkid_tid[i]);
#endif
            usleep(100);
        }
    }
}

int main() {
    create_worker_thread();
    create_worker_process();
    return 0;
}
