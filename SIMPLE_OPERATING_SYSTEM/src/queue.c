#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t *q)
{
    if (q == NULL)
        return 1;
    return (q->size == 0);
}

void enqueue(struct queue_t * q, struct pcb_t * proc) {
    if(q->size!= MAX_QUEUE_SIZE) {
        q->proc[q->size] = proc;
        q->size++;
    }
    else printf("FULL SLOT");
}

struct pcb_t *dequeue(struct queue_t *q)
{
    if (q->size == 0) return NULL;
    struct pcb_t* rm_proc = q->proc[0];
    for (int i = 0; i < q->size; i++) {
        if (i + 1 == MAX_QUEUE_SIZE) {
            q->proc[i] = NULL;
        }
        else q->proc[i] = q->proc[i + 1];
    }
    q->size--;
    return rm_proc;
}
