#include "Memory.h"

static uint8_t* last_free_block;

void init_memory()
{
    // Start at the beginning of unused ram (4096 byte aligned). Iterate until we reach the end of kernel-defined usable memory.
    for (uint8_t *ptr = START_OF_MEMBLOCK; (uint64_t)ptr < (uint64_t)END_OF_MEMORY; ptr+=4096)
    {   
        // Set block to Free (Not currently allocated / To be allocated)
        *ptr = MEM_FREE;
    }

    // Set for use in more efficient algorithms
    last_free_block = START_OF_MEMBLOCK;

    return;
}

void* kalloc()
{
    // Save the allocation and set allocated block as used.
    void* alloc = (void*)last_free_block;

    // If the free block is NULL (aka out of memory) then return NULL and skip finding new free block.
    //* kfree will eventually return more memory and reset the last_free_block variable.
    if (last_free_block == NULL)
        return NULL;

    *last_free_block = MEM_USED;
    while (true)
    {
        // Iterate over every block until we find a free block. If at any point the block is neither free nor used, panic.
        last_free_block += 4096;
        if ((uint64_t)last_free_block > (uint64_t)END_OF_MEMORY)
            last_free_block = NULL;


        if (*last_free_block == MEM_FREE)
            break;
        else if (*last_free_block != MEM_USED)
            kpanic(0xDEAD000A110C);
    }

    return alloc;
}

void kfree(void* ptr)
{
    // If ptr is not a valid beginning block ptr, panic.
    if (((uint64_t)ptr % 4096) != 0)
        kpanic(0xDEAD0000F433);
    
    // Mark block free
    *(uint8_t*)ptr = MEM_FREE;

    // If this block is less than the last free block, mark it as the last free block.
    if ((uint64_t)ptr < (uint64_t)last_free_block)
        last_free_block = ptr;

    return;
}