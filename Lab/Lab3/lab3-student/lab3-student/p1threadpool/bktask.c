#include "bktpool.h"

// retrieve a task from the task pool based on its ID.
struct bktask_t * bktask_get_byid(unsigned int bktaskid) {
  // Declares a pointer and initializes it with the current head of the linked list of tasks 
  struct bktask_t * ptask = bktask; 

  //  Check if out of the valid range 
  if (bktaskid < 0 || bktaskid > bktask_sz)
    return NULL; //  an invalid task ID.

  // Checks if the task pool is empty
  if (ptask == NULL)
    return NULL;

  // Iterates through the linked list of tasks 
  while (ptask != NULL) {
    // If found:
    if (ptask -> bktaskid == bktaskid)
      return ptask; // returns a pointer to that task

    //  If not, it moves to the next task in the list until the end of the list is reached.
    ptask = ptask -> tnext;
  }

  // If no matching task is found, the function returns NULL
  return NULL;
}

//  initializes a new task and assigns it a unique ID.
int bktask_init(unsigned int * bktaskid, void * func, void * arg) {
  // Allocates memory for a new task structure
  struct bktask_t * new_task = malloc(sizeof(struct bktask_t));

  // Assigns a unique task ID to the new task and increments the taskid_seed
  * bktaskid = taskid_seed++;
  bktask_sz++;

  // Assigns the provided function pointer (func), argument pointer (arg), and the generated task ID to the fields of the new task.
  new_task -> func = func;
  new_task -> arg = arg;
  new_task -> bktaskid = * bktaskid;

  // Links the new task to the existing task pool by setting its tnext pointer to the current head of the task pool. 
  new_task -> tnext = bktask;

  // Updates the task pool's head to the new task.
  bktask = new_task;

  return 0;
}