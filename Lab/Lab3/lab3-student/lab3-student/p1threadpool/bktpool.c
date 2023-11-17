#include "bktpool.h"

int bktpool_init()
{
   return bkwrk_create_worker(); // create a new worker thread in the task pool.
} 
