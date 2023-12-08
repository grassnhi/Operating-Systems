
#ifndef CPU_H       // checks whether the given macro CPU_H is not defined
// If it is not defined, the code between #ifndef and #endif is included in the final compiled code. 
// Otherwise, it is excluded.
#define CPU_H

#include "common.h"

/* Execute an instruction of a process. Return 0
 * if the instruction is executed successfully.
 * Otherwise, return 1. */
int run(struct pcb_t * proc);

#endif

