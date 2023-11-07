#include <stdio.h>

#define MAX_WORKER 10

int wrkid_busy[MAX_WORKER] = {0}; // Initialize all workers as not busy

void worker_wake(int wrkid, const char *msg) {
    printf("worker wake %d up %s\n", wrkid, msg);
}

void bkwrk_worker_func(int wrkid, const char *msg) {
    printf("Task func - Hello from %d\n", wrkid);
}

int bkwrk_create_worker() {
    int i;
    for (i = 0; i < MAX_WORKER; i++) {
        printf("bkwrk_create_worker got worker %d\n", 7593 + i);
    }
    return 0;
}

int bkwrk_get_worker() {
    int i;
    for (i = 0; i < MAX_WORKER; i++) {
        if (wrkid_busy[i] == 0) {
            return i;
        }
    }
    return -1;
}

int bkwrk_task_assign_worker(unsigned int bktask_id, unsigned int wrk_id) {
    if (wrk_id < 0 || wrk_id >= MAX_WORKER)
        return -1;

    // Advertise I AM WORKING
    wrkid_busy[wrk_id] = 1;
    printf("Assign tsk %d wrk %d\n", bktask_id, wrk_id);
    return 0;
}

int main() {
    int i;
    int task_id = 0;
    int wrk_id;

    // Simulating creating workers
    bkwrk_create_worker();

    // Simulating assigning tasks to workers with FIFO scheduler
    for (i = 0; i < 3; i++) {
        wrk_id = bkwrk_get_worker();
        if (wrk_id != -1) {
            bkwrk_task_assign_worker(task_id, wrk_id);
            if (i == 0) {
                worker_wake(wrk_id, "Task func - Hello from 1");
            } else {
                printf(">>>>>>>>>> Activate asynchronously\n");
            }
            task_id++;
        }
    }

    // Simulating waking up workers
    worker_wake(0, "Task func - Hello from 2");
    worker_wake(1, "Task func - Hello from 5");

    return 0;
}
