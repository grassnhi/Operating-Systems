#include <stdio.h>
#include "bktpool.h"
#include <unistd.h>

#undef STRESS_TEST

int func(void * arg) { // a void pointer argument (arg)
  int id = * ((int * ) arg); // casts it to an integer (id)

  printf("Task func - Hello from %d\n", id); // prints a message to the console
  fflush(stdout); //  flushes the standard output

  return 0;
}

int main(int argc, char * argv[]) {
  // Declares arrays to store task IDs, worker IDs, and task arguments for 15 tasks.
  int tid[15];
  int wid[15];
  int id[15];
  // Declares an integer variable ret to store return values from function calls.
  int ret;

  taskid_seed = 0;
  wrkid_cur = 0;
  bktask_sz = 0;

  ret = bktpool_init();
  // Calls the bktpool_init function to initialize the task pool
  if (ret != 0)
    return -1;

  
  // // Initializes three tasks with different IDs and associates them with the func function using bktask_init.
  id[0] = 1;
  bktask_init( & tid[0], & func, (void * ) & id[0]);
  id[1] = 2;
  bktask_init( & tid[1], & func, (void * ) & id[1]);
  id[2] = 5;
  bktask_init( & tid[2], & func, (void * ) & id[2]);

  
  wid[1] = bkwrk_get_worker();  // Gets a worker ID
  ret = bktask_assign_worker(tid[0], wid[1]); // Assigns the first task to the obtained worker 
  if (ret != 0) // if the assignment fails.
    printf("assign_task_failed tid=%d wid=%d\n", tid[0], wid[1]);

  bkwrk_dispatch_worker(wid[1]);

  // // Dispatches the worker with wid[1] to execute its assigned task using bkwrk_dispatch_worker.
  wid[0] = bkwrk_get_worker(); // // Gets a new worker ID and assigns the second task to this worker.
  ret = bktask_assign_worker(tid[1], wid[0]);
  if (ret != 0)
    printf("assign_task_failed tid=%d wid=%d\n", tid[1], wid[0]);

  // Gets a new worker ID and assigns the third task to this worker.
  wid[2] = bkwrk_get_worker(); 
  ret = bktask_assign_worker(tid[2], wid[2]);
  if (ret != 0)
    printf("assign_task_failed tid=%d wid=%d\n", tid[2], wid[2]);

  // Dispatches the workers with wid[0] and wid[2] to execute their assigned tasks
  bkwrk_dispatch_worker(wid[0]); 
  bkwrk_dispatch_worker(wid[2]);

  // lushes the standard output and sleeps for 1 second.
  fflush(stdout);
  sleep(1);

#ifdef STRESS_TEST
// This block is included only if STRESS_TEST is defined.
// It performs a stress test by initializing 15 tasks, getting worker IDs, assigning tasks to workers, and dispatching the workers to execute their tasks.
  int i = 0;
  for (i = 0; i < 15; i++) {
    id[i] = i;
    bktask_init( & tid[i], & func, (void * ) & id[i]);

    wid[i] = bkwrk_get_worker();
    ret = bktask_assign_worker(tid[i], wid[i]);

    if (ret != 0)
      printf("assign_task_failed tid=%d wid=%d\n", tid[i], wid[i]);

    bkwrk_dispatch_worker(wid[i]);
  }

  sleep(3);
#endif

  return 0;
}