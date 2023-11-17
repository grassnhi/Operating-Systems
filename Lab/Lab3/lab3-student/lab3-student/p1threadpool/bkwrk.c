#include "bktpool.h"
#include <signal.h>
#include <stdio.h>

#define _GNU_SOURCE
#include <linux/sched.h>
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <unistd.h>

//#define DEBUG
#define INFO          // it includes informational statements.
#define WORK_THREAD   // the code assumes thread-based worker creation and management.


// Defines the worker thread function.
// When a signal is received, it executes the task assigned to the worker and updates its status. 
// It takes a void pointer argument (arg), which is cast to an integer (i) representing the worker ID.
void * bkwrk_worker(void * arg) {
  // Declares variables related to signal handling 
  sigset_t set;
  int sig;
  int s;
  // and initializes i with the worker ID.
  int i = * ((int * ) arg); // Default arg is integer of workid
  struct bkworker_t * wrk = & worker[i];

  /* Taking the mask for waking up */  // Sets up a signal mask with SIGUSR1 and SIGQUIT.
  sigemptyset( & set);
  sigaddset( & set, SIGUSR1);
  sigaddset( & set, SIGQUIT);

#ifdef DEBUG
// Prints debug information if DEBUG is defined.
  fprintf(stderr, "worker %i start living tid %d \n", i, getpid());
  fflush(stderr);
#endif

  while (1) { // Infinite loop for the worker thread.
    /* wait for signal */
    s = sigwait( & set, & sig);
    if (s != 0)
      continue; //  If an error occurs, it continues to the next iteration.

#ifdef INFO
// Prints informational message if INFO is defined.
    fprintf(stderr, "worker wake %d up\n", i);
#endif

    /* Busy running */
    if (wrk -> func != NULL)  // If the worker has a task assigned (func is not NULL),
      wrk -> func(wrk -> arg); //  it executes the task.

    /* Advertise I DONE WORKING */
    wrkid_busy[i] = 0;
    worker[i].func = NULL;
    worker[i].arg = NULL;
    worker[i].bktaskid = -1;
  }
}

// Defines a function to assign a task to a worker.
int bktask_assign_worker(unsigned int bktaskid, unsigned int wrkid) {
  // Checks if the worker ID is within a valid range.
  if (wrkid < 0 || wrkid > MAX_WORKER)
    return -1;

  // Retrieves the task with the specified ID using bktask_get_byid
  struct bktask_t * tsk = bktask_get_byid(bktaskid);

  // // If the task is not found, returns -1.
  if (tsk == NULL)
    return -1; 

  /* Advertise I AM WORKING */
  wrkid_busy[wrkid] = 1;
  // Assigns the task to the worker, updates the worker's status, function, argument, and task ID.
  worker[wrkid].func = tsk -> func;
  worker[wrkid].arg = tsk -> arg;
  worker[wrkid].bktaskid = bktaskid;

  // Prints an informational message and returns 0 to indicate success.
  printf("Assign tsk %d wrk %d \n", tsk -> bktaskid, wrkid);
  return 0;
}

// Defines a function to create worker threads.
// For thread-based workers, it uses clone to create threads.
// For fork-based workers, need to implement the fork version of worker creation.
int bkwrk_create_worker() {
  unsigned int i;

  // Loops through the worker threads.
  for (i = 0; i < MAX_WORKER; i++) {
#ifdef WORK_THREAD
    void ** child_stack = (void ** ) malloc(STACK_SIZE);
    unsigned int wrkid = i;
    pthread_t threadid;

    sigset_t set;
    int s;

    // setting up the signal mask.
    sigemptyset( & set);
    sigaddset( & set, SIGQUIT);
    sigaddset( & set, SIGUSR1);
    sigprocmask(SIG_BLOCK, & set, NULL);

    /* Stack grow down - start at top*/
    void * stack_top = child_stack + STACK_SIZE;

    // Creates a worker thread using clone
    wrkid_tid[i] = clone( & bkwrk_worker, stack_top,
      CLONE_VM | CLONE_FILES,
      (void * ) & i);
#ifdef INFO
    fprintf(stderr, "bkwrk_create_worker got worker %u\n", wrkid_tid[i]);
#endif

    usleep(100);

    #else

    /* TODO: Implement fork version of create worker */
    pid_t pid = fork();
    if(pid == 0){ // Child process
      // Set up the signal mask for the child process
      sigset_t set;
      sigemptyset(&set);
      sigaddset(&set, SIGQUIT);
      sigaddset(&set, SIGUSR1);
      sigprocmask(SIG_BLOCK, &set, NULL);
      // This ensures that these signals are blocked during the execution of the child process.

      // Execute the worker function
      bkwrk_worker((void *)&i); // This allows the child process to know which worker it corresponds to.

      // Exit when done to prevent child process from running the main program logic
      exit(EXIT_SUCCESS);
    }else if(pid > 0){    // Parent process
      // Save the child's PID
      wrkid_tid[i] = pid; 
      // This allows the parent process to keep track of the process ID of each worker it has created.
    }
#endif

  }
  // Ends the worker creation function, returning 0.
  return 0;
}

// This function is part of a scheduler (still needs implementation) to select an available worker.
int bkwrk_get_worker() {
  /* TODO Implement the scheduler to select the resource entity 
   * The return value is the ID of the worker which is not currently 
   * busy or wrkid_busy[1] == 0 
   */

  for (unsigned int i = 0; i < MAX_WORKER; i++)
  {
    // Check if the current worker is not busy
    if(wrkid_busy[i] == 0){
      // Mask the worker as busy
      wrkid_busy[i] = 1;

      // Return the ID of the available worker
      return i;
    }
  }
  
  return -1; // No worker is available
  //return 0;
}

// Defines a function to dispatch a task to a worker.
int bkwrk_dispatch_worker(unsigned int wrkid) {

#ifdef WORK_THREAD
// For thread-based workers (WORK_THREAD is defined), it gets the thread ID and sends a signal (SIG_DISPATCH)
  unsigned int tid = wrkid_tid[wrkid];

  /* Invalid task */
  if (worker[wrkid].func == NULL)
    return -1;

#ifdef DEBUG
// If DEBUG is defined, it prints a debug message.
  fprintf(stderr, "brkwrk dispatch wrkid %d - send signal %u \n", wrkid, tid);
#endif
//  For thread-based workers, it sends a signal (SIG_DISPATCH) using syscall(SYS_tkill)
  syscall(SYS_tkill, tid, SIG_DISPATCH);
  #else
  /* TODO: Implement fork version to signal worker process here */

  // Assuming wrkid_tid stores PIDs for forked process
  pid_t pid = wrkid_tid[wrkid]; // Retrieves the process ID

  // Invalid task
  if(worker[wrkid].func == NULL){
    return -1; // If there is no valid task, it returns -1 to indicate an error.
  }
  //  For fork-based workers, it sends a signal using kill
  if(kill(pid, SIG_DISPATCH) != 0){
    // If the kill function returns a non-zero value, it indicates an error. 
    perror("Error sending signal to worker");
    return -1;
  }
  // dispatching a task to a fork-based worker by sending a signal to the corresponding process.
  // If any issues occur during this process, it provides an error message and returns -1.
#endif
}