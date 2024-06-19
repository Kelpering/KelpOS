#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "KLib.h"

#define ALLOC_OFFSET(start, x) (((uint64_t) start) + (((uint64_t) x) * 1024))
#define BLOCK_OFFSET(start, ptr) (((uint64_t)ptr - (uint64_t)start) / 1024)


void init_memory();
void* kalloc();
void kfree(void* ptr);

#endif // _MEMORY_H_