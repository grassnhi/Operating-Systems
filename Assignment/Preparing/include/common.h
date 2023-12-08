#ifndef COMMON_H
#define COMMON_H

/* Define structs and routine could be used by every source files */

#include <stdint.h>

#ifndef OSCFG_H
#include "os-cfg.h"
#endif

#ifndef OSMM_H
#include "os-mm.h"
#endif

#define ADDRESS_SIZE	20		// the number of bits in a virtual address.
#define OFFSET_LEN	10			// number of bits used to represent the offset within a page.
#define FIRST_LV_LEN	5		// the indices of the first level of the page table
#define SECOND_LV_LEN	5		// the indices of second layers of the page table
#define SEGMENT_LEN     FIRST_LV_LEN
#define PAGE_LEN        SECOND_LV_LEN

#define NUM_PAGES	(1 << (ADDRESS_SIZE - OFFSET_LEN))	// the total number of pages in the virtual address space
					// NUM_PAGES == to 2^(ADDRESS_SIZE - OFFSET_LEN)
#define PAGE_SIZE	(1 << OFFSET_LEN)					// the size of each page in bytes.
					// PAGE_SIZE == to 2^OFFSET_LEN

enum ins_opcode_t {
	CALC,	// Just perform calculation, only use CPU
	ALLOC,	// Allocate memory
	FREE,	// Deallocated a memory block
	READ,	// Write data to a byte on memory
	WRITE	// Read data from a byte on memory
};

/* instructions executed by the CPU */
struct inst_t {					// represents an instruction that is executed by the CPU
	enum ins_opcode_t opcode;	//  the type of instruction
	uint32_t arg_0; 			// Argument lists for instructions
	uint32_t arg_1;
	uint32_t arg_2;
};

struct code_seg_t {			// the code segment of a process. 
	struct inst_t * text;	// a text field that points to an array of inst_t structures (instruction)
	uint32_t size;			// the number of instructions in the code segment
};

struct trans_table_t {
	/* A row in the page table of the second layer */
	struct  {
		addr_t v_index; // The index of virtual address
		addr_t p_index; // The index of physical address
	} table[1 << SECOND_LV_LEN];	// an array of struct objects 
			// each of which contains an index of a virtual address and an index of a physical address.
	int size;			// the number of rows in the table.
};

/* Mapping virtual addresses and physical ones */
struct page_table_t {
	/* Translation table for the first layer */
	struct {
		addr_t v_index;	// Virtual index
		struct trans_table_t * next_lv;
	} table[1 << FIRST_LV_LEN];	// (size = 2^FIRST_LV_LEN)
	int size;	// Number of row in the first layer
};

/* PCB, describe information about a process */
struct pcb_t {
	uint32_t pid;	// PID
	uint32_t priority; // Default priority, this legacy (FIXED) value depend on process itself
	struct code_seg_t * code;	// Code segment
	addr_t regs[10]; // Registers, store address of allocated regions
	uint32_t pc; 

#ifdef MLQ_SCHED	// checks whether the macro MLQ_SCHED is defined or not
	// Priority on execution (if supported), on-fly aka. changeable
	// and this vale overwrites the default priority when it existed
	uint32_t prio;     
#endif // If defined, above is included in the final compiled code.

#ifdef MM_PAGING  // checks whether the macro MM_PAGING is defined 
	struct mm_struct *mm;				// a pointer to a mm_struct structure (the memory management of a process)
	struct memphy_struct *mram;			// a pointer to a memphy_struct structure (a physical memory region)
	struct memphy_struct **mswp;		// a pointer to a pointer to a memphy_struct structure -> used to represent the swap space of a process
	struct memphy_struct *active_mswp;	// a pointer to a memphy_struct -> the active swap space of a process
#endif
	struct page_table_t * page_table; // Page table -> a pointer to a page_table_t structure
	uint32_t bp;	// Break pointer

};

#endif

