#ifndef BKTPOOL_H
#define BKTPOOL_H

#include <stdlib.h>

#include <pthread.h>

#define MAX_WORKER 10 // maximum number of workers in the task pool
#define WRK_THREAD 1  // the number of threads each worker can have
#define STACK_SIZE 4096
#define SIG_DISPATCH SIGUSR1 // a signal used for worker thread dispatching

/* Task ID is unique non-decreasing integer */
int taskid_seed;  // generating unique task IDs.

int wrkid_tid[MAX_WORKER];  //array used to map worker IDs to thread IDs.
int wrkid_busy[MAX_WORKER]; // track the busy status of each worker.
int wrkid_cur;              // keep track of the current worker ID.

struct bktask_t {             //  a task in the task pool
  void( * func)(void * arg);
  void * arg;
  unsigned int bktaskid;
  struct bktask_t * tnext;
}* bktask;

int bktask_sz;

struct bkworker_t {         // store information about each worker.
  void( * func)(void * arg);
  void * arg;
  unsigned int wrkid;
  unsigned int bktaskid;
};

struct bkworker_t worker[MAX_WORKER];

/* Prototype API */

/* bktpool module */
int bktpool_init();   //  initialize the task pool.

/* bktask module */
struct bktask_t * bktask_get_byid(unsigned int bktaskid); // retrieves a task by its ID.
int bktask_init(unsigned int * bktaskid, void * func, void * arg); // initializes a task with a function pointer and an argument.
int bktask_assign_worker(unsigned int bktaskid, unsigned int wrkid); // assigns a task to a specific worker.

/* bkwrk module */
void * bkwrk_worker(void * arg);  // the worker thread's entry point.
int bkwrk_create_worker();        // reates a new worker.
int bkwrk_dispatch_worker(unsigned int wrkid); // dispatches a task to a specific worker.
int bkwrk_get_worker();           // retrieves a worker.

#endif