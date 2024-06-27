#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "KLib.h"

// Start memblock at Kernel end, then align to the next 4096 byte boundary
#define START_OF_MEMBLOCK ((void*)(((uint64_t)END_OF_KERNEL + 0xFFF) & ~0xFFFL))
// End of usable ram by the kernel
#define END_OF_MEMORY ((void*)(START_OF_KERNEL+(4096*1024)))
#define MEM_FREE 0x80
#define MEM_USED 0x40


void init_memory();
void* kalloc();
void kfree(void* ptr);

#endif // _MEMORY_H_