#include <signal.h>
#include <stdio.h>

#define GNU_SOURCE

#include <linux/sched.h>
#include <sys/syscall.h>
#include <unistd.h>

#define INFO
#define WORKTHREAD

int bkwrk_create_worker() {
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

        // Stack grow down - start at top
        void *stack_top = child_stack + STACK_SIZE;

        wrkid_tid[i] = clone(&bkwrk_worker, stack_top, CLONE_VM | CLONE_FILES, (void *)&i);
#ifdef INFO
        fprintf(stderr, "bkwrk_create_worker got worker %u\n", wrkid_tid[i]);
#endif
        usleep(100);
#else
    }
}
