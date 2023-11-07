/*
*   From bktpool.h
*/

#include <stdlib.h>
#include <pthread.h>

#define MAX_WORKER  10

#define WRK_THREAD  1
#define STACK_SIZE  4096

#define SIG_DISPATCH SIGUSRI

typedef void *(*thread_func_t)(void *);

/* Task ID is unique non-decreasing integer */

int taskid_seed;

int wrkid_tid[MAX_WORKER];
int wrkid_busy[MAX_WORKER];
int wrkid_cur;

struct bktask_t
{
    void (*func)(void * arg);
    void *arg;
    unsigned int bktaskid;
    struct bktask_t *tnext;
} *bktask;

int bktask_sz;

struct bkworker_t
{
    void (*func)(void *arg);
    void *arg;
    unsigned int wrkid;
    unsigned int bktaskid;
};

struct bkworker_t worker[MAX_WORKER];

/*
*   From bktpool.c
*/

#include "bktpool.h"

int bktpool_init(){
    return bkwrk_create_worker();
}
