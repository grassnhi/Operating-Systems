#ifndef OSMM_H
#define OSMM_H

#include <pthread.h>
#include <sys/types.h>

#define MTX
#define MM_PAGING
#define PAGING_MAX_MMSWP 4 /* max number of supported swapped space */
#define PAGING_MAX_SYMTBL_SZ 30

typedef char BYTE;
typedef uint32_t addr_t;
// typedef unsigned int uint32_t;

struct pgn_t
{
   int pgn;
   struct pgn_t *pg_next;
};

/*
 *  Memory region struct
 */
struct vm_rg_struct
{
   unsigned long rg_start;
   unsigned long rg_end;

   struct vm_rg_struct *rg_next; // Unuse for this fucking Project
};

/*
 *  Memory area struct
 */
struct vm_area_struct
{ // Linked list
   unsigned long vm_id;
   unsigned long vm_start;
   unsigned long vm_end;

   unsigned long sbrk;
   /*
    * Derived field
    * unsigned long vm_limit = vm_end - vm_start
    */
   struct mm_struct *vm_mm;
   struct vm_rg_struct *vm_freerg_list;
   struct vm_area_struct *vm_next;
};

struct LRU_node
{
   int LRU_pgn;
   struct LRU_node *next_pgn;
};

/*
 * Memory management struct
 */
struct mm_struct
{
   uint32_t *pgd; // could be an array

   struct LRU_node *LRU_stack_head; // head of the LRU stack

   struct vm_area_struct *mmap;

   /* Currently we support a fixed number of symbol */
   struct vm_rg_struct symrgtbl[PAGING_MAX_SYMTBL_SZ];
   
   /* Array of allocated reg */
   //int allocated[PAGING_MAX_SYMTBL_SZ];

   /* list of free page */
   struct pgn_t *fifo_pgn;
};

/*
 * FRAME/MEM PHY struct
 */
struct framephy_struct
{
   int fpn;
   struct framephy_struct *fp_next;

   /* Resereed for tracking allocated framed */
   struct mm_struct *owner;
};

struct memphy_struct
{
   /* Basic field of data and size */
   BYTE *storage;
   int maxsz;

   /* Sequential device fields */
   int rdmflg; // Flag to determine whether randomly access or not
   int cursor;
   
#ifdef MTX
   /* pthread for locking when get free frame*/
   pthread_mutex_t access_lock;
#endif
   
   /* Management structure */
   struct framephy_struct *free_fp_list;
   struct framephy_struct *used_fp_list;
};

#endif
