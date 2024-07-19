#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "Klib.h"
#include "Riscv.h"


#define PG_SIZE 4096    // The size of a standard page in bytes
#define PG_NUM 1024     // The number of pages available to the OS
#define START_OF_MEM ((void*)(((uint64_t)END_OF_KERNEL + 0xFFF) & ~0xFFFL)) // Start of free, contiguous ram
#define END_OF_MEM ((void*)(START_OF_KERNEL+(PG_SIZE*PG_NUM)))              // End of ram (statically defined)

struct free_pg
{
    struct free_pg *prev;
    struct free_pg *next;
};
typedef struct free_pg free_pg;

void init_memory();

void *kalloc();
void *zalloc();
void kfree(void *page);


#endif // __MEMORY_H__