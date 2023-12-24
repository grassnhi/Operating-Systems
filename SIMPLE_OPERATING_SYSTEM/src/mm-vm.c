// #ifdef MM_PAGING
/*
 * PAGING based Memory Management
 * Virtual memory module mm/mm-vm.c
 */

#include "string.h"
#include "mm.h"
#include <stdlib.h>
#include <stdio.h>

/*enlist_vm_freerg_list - add new rg to freerg_list
 *@mm: memory region
 *@rg_elmt: new region
 *
 */
int enlist_vm_freerg_list(struct mm_struct *mm, struct vm_rg_struct *rg_elmt)
{
  struct vm_rg_struct *rg_node = mm->mmap->vm_freerg_list;

  if (rg_elmt->rg_start >= rg_elmt->rg_end)
    return -1;

  if (rg_node != NULL)
    rg_elmt->rg_next = rg_node;

  /* Enlist the new region */
  mm->mmap->vm_freerg_list = rg_elmt;

  return 0;
}

/*get_vma_by_num - get vm area by numID
 *@mm: memory region
 *@vmaid: ID vm area to alloc memory region
 *
 */
struct vm_area_struct *get_vma_by_num(struct mm_struct *mm, int vmaid)
{
  struct vm_area_struct *pvma = mm->mmap;

  if (mm->mmap == NULL)
    return NULL;

  int vmait = 0;

  while (vmait < vmaid)
  {
    if (pvma == NULL)
      return NULL;

    pvma = pvma->vm_next;
  }

  return pvma;
}

/*get_symrg_byid - get mem region by region ID
 *@mm: memory region
 *@rgid: region ID act as symbol index of variable
 *
 */
struct vm_rg_struct *get_symrg_byid(struct mm_struct *mm, int rgid)
{
  if (rgid < 0 || rgid > PAGING_MAX_SYMTBL_SZ)
    return NULL;

  return &mm->symrgtbl[rgid];
}

/*__alloc - allocate a region memory
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@rgid: memory region ID (used to identify variable in symbole table)
 *@size: allocated size
 *@alloc_addr: address of allocated memory region
 *
 */
int __alloc(struct pcb_t *caller, int vmaid, int rgid, int size, int *alloc_addr)
{
  /*Allocate at the toproof */
    struct vm_rg_struct rgnode; // Only use this when exist free region
    
    // Exist free vm_rg in the listl
    if (get_free_vmrg_area(caller, vmaid, size, &rgnode) == 0)
    {
        caller->mm->symrgtbl[rgid].rg_start = rgnode.rg_start;
        caller->mm->symrgtbl[rgid].rg_end = rgnode.rg_end;
        
        *alloc_addr = rgnode.rg_start;
        
        return 0;
    }
    // Don't exist then increase the size of vm_area to create a new rg
    /* TODO get_free_vmrg_area FAILED handle the region management (Fig.6)*/
    
    /*Attempt to increate limit to get space */
    struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
    //int inc_sz = PAGING_PAGE_ALIGNSZ(size); 
    // int inc_limit_ret
    int old_sbrk;
    
    old_sbrk = cur_vma->sbrk;
    
    /* TODO INCREASE THE LIMIT
    * inc_vma_limit(caller, vmaid, inc_sz)
    */
    if (inc_vma_limit(caller, vmaid, size) == 0)
    {
        /*Successful increase limit */
        caller->mm->symrgtbl[rgid].rg_start = old_sbrk;
        caller->mm->symrgtbl[rgid].rg_end = old_sbrk + size;
        
        *alloc_addr = old_sbrk;
        caller->mm->mmap->sbrk = old_sbrk + size;
        
        return 0;
    }
    else
        return -1;
}

/*__free - remove a region memory
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@rgid: memory region ID (used to identify variable in symbole table)
 *@size: allocated size
 *
 */
int __free(struct pcb_t *caller, int vmaid, int rgid)
{

  if (rgid < 0 || rgid > PAGING_MAX_SYMTBL_SZ)
    return -1;

  /* TODO: Manage the collect freed region to freerg_list */
  struct vm_rg_struct *regRG = get_symrg_byid(caller->mm, rgid);
  if (regRG->rg_end <= regRG->rg_start) {
  #ifdef MY_CHECK
        printf("Segmentation fault\n");
  #endif
        return -1;
    } 
  struct vm_rg_struct *headFreeList = caller->mm->mmap->vm_freerg_list;
  while (headFreeList)
  {
    if (headFreeList->rg_start == regRG->rg_end)
    {
      headFreeList->rg_start = regRG->rg_start;
      regRG->rg_start = regRG->rg_end = 0;
      return 0;
    }
    else if (headFreeList->rg_end == regRG->rg_start) {
      headFreeList->rg_end = regRG->rg_end;
      regRG->rg_start = regRG->rg_end = 0;
      return 0;
    }
    headFreeList = headFreeList->rg_next;
  }
  struct vm_rg_struct *rgnode = (struct vm_rg_struct*)malloc(sizeof(struct vm_rg_struct));
  rgnode->rg_start = regRG->rg_start;
  rgnode->rg_end = regRG->rg_end;
  rgnode->rg_next = NULL;
  // Remember to handle case like this (0,100)->(100, 300)->NULL into (0, 300)->NULL
  // Reset that symbol table
  regRG->rg_start = regRG->rg_end = 0;
  /*enlist the obsoleted memory region */
  enlist_vm_freerg_list(caller->mm, rgnode);

  return 0;
}

/*pgalloc - PAGING-based allocate a region memory
 *@proc:  Process executing the instruction
 *@size: allocated size
 *@reg_index: memory region ID (used to identify variable in symbole table)
 */
int pgalloc(struct pcb_t *proc, uint32_t size, uint32_t reg_index)
{
  int addr;

  /* By default using vmaid = 0 */
  //return __alloc(proc, 0, reg_index, size, &addr);
  int returned_val = __alloc(proc, 0, reg_index, size, &addr);
  if(returned_val < 0) {
#ifdef MY_CHECK
    printf("allocating error: invalid size value\n");
#endif
    return returned_val;
  }
#ifdef IO_DUMP
  printf("allocate region=%d reg=%d\n", size, reg_index);
#ifdef PAGETBL_DUMP
  print_pgtbl(proc, 0, -1); //print max TBL
#endif
#ifdef MY_CHECK
  print_list_rg(proc->mm->mmap->vm_freerg_list);
#endif
#endif

  return returned_val;
}

/*pgfree - PAGING-based free a region memory
 *@proc: Process executing the instruction
 *@size: allocated size
 *@reg_index: memory region ID (used to identify variable in symbole table)
 */

int pgfree_data(struct pcb_t *proc, uint32_t reg_index)
{
  //return __free(proc, 0, reg_index);
  int returned_val = __free(proc, 0, reg_index);
  if(returned_val < 0) {
    printf("free error: double free\n");
    return returned_val;
  }
#ifdef IO_DUMP
  printf("free reg=%d\n", reg_index);
#ifdef PAGETBL_DUMP
  print_pgtbl(proc, 0, -1); //print max TBL
#endif
#ifdef MY_CHECK
  print_list_rg(proc->mm->mmap->vm_freerg_list);
#endif
#endif
  return returned_val;
}

/*pg_getpage - get the page in ram
 *@mm: memory region
 *@pagenum: PGN
 *@framenum: return FPN
 *@caller: caller
 *
 */
int pg_getpage(struct mm_struct *mm, int pgn, int *fpn, struct pcb_t *caller)
{
  uint32_t pte = mm->pgd[pgn];
  int tgtfpn = PAGING_SWP(pte); // the target frame storing our variable
  // pte: change from SWAP mode to MEM mode

  if (!PAGING_PAGE_PRESENT(pte)) // The page we want is in the backing store
  {                              /* Page is not online, make it actively living */
    // Two case, exist free frame and not exist free frame.
    // There is free frame in MEM
    int fpn;
    if (MEMPHY_get_freefp(caller->mram, &fpn) == 0)
    {
      pte_set_fpn(&mm->pgd[pgn], fpn);
      __swap_cp_page(caller->active_mswp, tgtfpn, caller->mram, fpn);
    }
    else
    {
      int vicpgn, swpfpn;
      // vicpgn: pgn will change from MEM mode to SWAP mode
      // swpfpn : fpn in SWAP that vicpgn will put into
      // int vicfpn;
      // uint32_t vicpte;

      /* TODO: Play with your paging theory here */

      /* Find victim page */
      if (find_victim_page(caller->mm, &vicpgn) != 0)
      	return -1;

      /* Get free frame in MEMSWP */
      MEMPHY_get_freefp(caller->active_mswp, &swpfpn);

      /*Get frame from victim page*/
      int vicfpn = PAGING_FPN(vicpgn);

      /* Do swap frame from MEMRAM to MEMSWP and vice versa*/
      /* Copy victim frame to swap */
      __swap_cp_page(caller->mram, vicfpn, caller->active_mswp, swpfpn);
      // Update the victim page
      /* Copy target frame from swap to mem */
      __swap_cp_page(caller->active_mswp, tgtfpn, caller->mram, vicfpn);

      /* Update page table */
      // Change vicpgn
      pte_set_swap(&caller->mm->pgd[vicpgn], 0, swpfpn);

      /* Update its online status of the target page */
      // update the pte
      pte_set_fpn(&caller->mm->pgd[pgn], vicfpn);
    }

    enlist_pgn_node(&caller->mm->fifo_pgn, pgn);
  }

  *fpn = PAGING_FPN(pte);

  return 0;
}

/*pg_getval - read value at given offset
 *@mm: memory region
 *@addr: virtual address to acess
 *@value: value
 *
 */
int pg_getval(struct mm_struct *mm, int addr, BYTE *data, struct pcb_t *caller)
{
  int pgn = PAGING_PGN(addr);
  int off = PAGING_OFFST(addr);
  int fpn;

  /* Get the page to MEMRAM, swap from MEMSWAP if needed */
  if (pg_getpage(mm, pgn, &fpn, caller) != 0) {
  #ifdef MY_CHECK
        printf("Segmentation fault\n");
  #endif
    return -1; /* invalid page access */
  }

  //Update the pgn that just reference
  update_LRU(mm, pgn);

  int phyaddr = (fpn << PAGING_ADDR_FPN_LOBIT) + off;

  MEMPHY_read(caller->mram, phyaddr, data);

  return 0;
}

/*pg_setval - write value to given offset
 *@mm: memory region
 *@addr: virtual address to acess
 *@value: value
 *
 */
int pg_setval(struct mm_struct *mm, int addr, BYTE value, struct pcb_t *caller)
{
  int pgn = PAGING_PGN(addr);   // Get page number
  int off = PAGING_OFFST(addr); // Get offset
  int fpn;

  /* Get the page to MEMRAM, swap from MEMSWAP if needed */
  if (pg_getpage(mm, pgn, &fpn, caller) != 0) {
    #ifdef MY_CHECK
        printf("Segmentation fault\n");
    #endif
    return -1; /* invalid page access */
  }

  // update the LRU stack
  update_LRU(mm, pgn);
  int phyaddr = (fpn << PAGING_ADDR_FPN_LOBIT) + off;

  MEMPHY_write(caller->mram, phyaddr, value);

  return 0;
}

/*__read - read value in region memory
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@offset: offset to acess in memory region
 *@rgid: memory region ID (used to identify variable in symbole table)
 *@size: allocated size
 *
 */
int __read(struct pcb_t *caller, int vmaid, int rgid, int offset, BYTE *data)
{
  struct vm_rg_struct *currg = get_symrg_byid(caller->mm, rgid);

  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  if (currg == NULL || cur_vma == NULL || (currg->rg_start == 0 && currg->rg_end == 0)) /* Invalid memory identify */
    return -1;
  if(currg->rg_start + offset >= currg->rg_end) {
    return -1;
  }
//  if ()
  //  return -1000; /* Undefined vm region */

  pg_getval(caller->mm, currg->rg_start + offset, data, caller);

  return 0;
}

/*pgwrite - PAGING-based read a region memory */
int pgread(
    struct pcb_t *proc, // Process executing the instruction
    uint32_t source,    // Index of source register
    uint32_t offset,    // Source address = [source] + [offset]
    uint32_t destination)
{
  BYTE data;
  int returned_val = __read(proc, 0, source, offset, &data);
  if(returned_val < 0) {
#ifdef MY_CHECK
    printf("reading error: not allocated or out of region range\n");
#endif
    return returned_val;
  }
  destination = (uint32_t)data;
#ifdef IODUMP
  printf("read region=%d offset=%d value=%d\n", source, offset, data);
#ifdef PAGETBL_DUMP
  print_pgtbl(proc, 0, -1); // print max TBL
#endif
#ifdef MY_CHECK
  MEMPHY_dump(proc->mram);
#endif
#endif

  return returned_val;
}

/*__write - write a region memory
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@offset: offset to acess in memory region
 *@rgid: memory region ID (used to identify variable in symbole table)
 *@size: allocated size
 *
 */
int __write(struct pcb_t *caller, int vmaid, int rgid, int offset, BYTE value)
{
  struct vm_rg_struct *currg = get_symrg_byid(caller->mm, rgid);

  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  if (currg == NULL || cur_vma == NULL) /* Invalid memory identify */
    return -1;
  
  if(currg->rg_start + offset >= currg->rg_end) {
    return -1;
  }

  pg_setval(caller->mm, currg->rg_start + offset, value, caller);

  return 0;
}

/*pgwrite - PAGING-based write a region memory */
int pgwrite(
    struct pcb_t *proc,   // Process executing the instruction
    BYTE data,            // Data to be wrttien into memory
    uint32_t destination, // Index of destination register
    uint32_t offset)
{
 int returned_val = __write(proc, 0, destination, offset, data);
  if(returned_val < 0) {
#ifdef MY_CHECK
    printf("writing error: not allocated or out of region range\n");
#endif
    return returned_val;
  }
#ifdef IODUMP
  printf("write region=%d offset=%d value=%d\n", destination, offset, data);
#ifdef PAGETBL_DUMP
  print_pgtbl(proc, 0, -1); // print max TBL
#endif
#ifdef MY_CHECK
  MEMPHY_dump(proc->mram);
#endif
#endif

  return __write(proc, 0, destination, offset, data);
}

/*free_pcb_memphy - collect all memphy of pcb
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@incpgnum: number of page
 */
int free_pcb_memph(struct pcb_t *caller)
{
  int pagenum, fpn;
  uint32_t pte;

  for (pagenum = 0; pagenum < PAGING_MAX_PGN; pagenum++)
  {
    pte = caller->mm->pgd[pagenum];

    if (!PAGING_PAGE_PRESENT(pte))
    {
      fpn = PAGING_FPN(pte);
      MEMPHY_put_freefp(caller->mram, fpn);
    }
    else
    {
      fpn = PAGING_SWP(pte);
      MEMPHY_put_freefp(caller->active_mswp, fpn);
    }
  }

  return 0;
}

/*get_vm_area_node - get vm area for a number of pages
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@incpgnum: number of page
 *@vmastart: vma end
 *@vmaend: vma end
 *
 */
struct vm_rg_struct *get_vm_area_node_at_brk(struct pcb_t *caller, int vmaid, int size, int alignedsz)
{
  struct vm_rg_struct *newrg;
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  newrg = malloc(sizeof(struct vm_rg_struct));

  newrg->rg_start = cur_vma->sbrk;
  newrg->rg_end = newrg->rg_start + size;

  return newrg;
}

/*validate_overlap_vm_area
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@vmastart: vma end
 *@vmaend: vma end
 *
 */
int validate_overlap_vm_area(struct pcb_t *caller, int vmaid, int vmastart, int vmaend)
{
  	//struct vm_area_struct * vma = caller -> mm -> mmap ;
	/* TODO validate the planned memory area is not overlapped */
	// We will do nothing because we only use 1 vm_area
	return 0;
}

/*inc_vma_limit - increase vm area limits to reserve space for new variable only when
 *vm_end - sbrk is not large enough to allocate a variable size.
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@inc_sz: increment size
 *
 */
int inc_vma_limit(struct pcb_t *caller, int vmaid, int regSize)
{
  struct vm_rg_struct *newrg = malloc(sizeof(struct vm_rg_struct));
  // regSize < 1 pageSize -> inc_amt = 1 pageSize
  // regSize > 1 pageSize && < 2 pageSize -> inc_amt = 2 pageSize
  int inc_amt = PAGING_PAGE_ALIGNSZ(regSize);
  int incnumpage = inc_amt / PAGING_PAGESZ;
  struct vm_rg_struct *area = get_vm_area_node_at_brk(caller, vmaid, regSize, inc_amt);
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  int old_end = cur_vma->vm_end;

  /*Validate overlap of obtained region */
  if (validate_overlap_vm_area(caller, vmaid, area->rg_start, area->rg_end) < 0)
    return -1; /*Overlap and failed allocation */

  if (cur_vma->vm_end - cur_vma->sbrk < regSize)
  {
    /* The obtained vm area (only)
     * now will be alloc real ram region */
    cur_vma->vm_end += inc_amt;
    if (vm_map_ram(caller, area->rg_start, area->rg_end,
                   old_end, incnumpage, newrg) < 0)
      return -1; /* Map the memory to MEMRAM */
  }
  // Increase the sbrk will be handled by __alloc()

  return 0;
}

/*update LRU stack
  Use for ALLOC, READ, WRITE operations
*/
void update_LRU(struct mm_struct *mm, int pgn)
{
  // Check if that pgn exist, then put it on top
  struct LRU_node **stackLRU = &(mm->LRU_stack_head);
  while (*stackLRU)
  {
    if ((*stackLRU)->LRU_pgn == pgn)
    {
      struct LRU_node *topPgn = *stackLRU;
      *stackLRU = (*stackLRU)->next_pgn;
      topPgn->next_pgn = mm->LRU_stack_head;
      mm->LRU_stack_head = topPgn;
      return;
    }
    else
      stackLRU = &(*stackLRU)->next_pgn;
  }

  // That is the first pgn ever exist in a stack
  struct LRU_node *newPgn = (struct LRU_node *)malloc(sizeof(struct LRU_node));
  newPgn->LRU_pgn = pgn;
  newPgn->next_pgn = mm->LRU_stack_head;
  mm->LRU_stack_head = newPgn;
}

/*
  Take the last pgn in the stack to be the victim page
*/
struct LRU_node *LRU_last_node(struct mm_struct *mm)
{
  struct LRU_node **stackLRU = &(mm->LRU_stack_head);
  while (*stackLRU)
  {
    if (!(*stackLRU)->next_pgn)
      break;
    stackLRU = &(*stackLRU)->next_pgn;
  }
  struct LRU_node *last_node = *stackLRU;
  *stackLRU = NULL;
  return last_node;
}

/*find_victim_page - find victim page
 *@caller: caller
 *@pgn: return page number
 *
 */
int find_victim_page(struct mm_struct *mm, int *retpgn)
{
  struct pgn_t *pg = mm->fifo_pgn;

  /* TODO: Implement the theorical mechanism to find the victim page */
  struct LRU_node *last_node = LRU_last_node(mm);
  *retpgn = last_node->LRU_pgn;

  free(pg);

  return 0;
}

/*get_free_vmrg_area - get a free vm region
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@size: allocated size
 *
 */
int get_free_vmrg_area(struct pcb_t *caller, int vmaid, int size, struct vm_rg_struct *newrg)
{
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  struct vm_rg_struct *rgit = cur_vma->vm_freerg_list;

  if (rgit == NULL)
    return -1;

  /* Probe unintialized newrg */
  newrg->rg_start = newrg->rg_end = -1;

  /* Traverse on list of free vm region to find a fit space */
  while (rgit != NULL)
  {
    if (rgit->rg_start + size <= rgit->rg_end)
    { /* Current region has enough space */
      newrg->rg_start = rgit->rg_start;
      newrg->rg_end = rgit->rg_start + size;

      /* Update left space in chosen region */
      if (rgit->rg_start + size < rgit->rg_end)
      {
        rgit->rg_start = rgit->rg_start + size;
        break;
      }
      else
      { /*Use up all space, remove current node */
        /*Clone next rg node */
        struct vm_rg_struct *nextrg = rgit->rg_next;

        /*Cloning */
        if (nextrg != NULL)
        {
          rgit->rg_start = nextrg->rg_start;
          rgit->rg_end = nextrg->rg_end;

          rgit->rg_next = nextrg->rg_next;

          free(nextrg);
        }
        else
        {                                /*End of free list */
          rgit->rg_start = rgit->rg_end; // dummy, size 0 region
          rgit->rg_next = NULL;
        }
        break;
      }
    }
    else
    {
      rgit = rgit->rg_next; // Traverse next rg
    }
  }

  if (newrg->rg_start == -1) // new region not found
    return -1;

  return 0;
}

// #endif
