#include "Memory.h"

void* START_OF_MEMBLOCK;
volatile static uint64_t bitfield_block;


void init_memory()
{
    START_OF_MEMBLOCK = (void*) (((uint64_t)END_OF_KERNEL + 0xFF) & ~0xFFL);
    bitfield_block = 0;

    return;
}

void* kalloc()
{
    // Loop until we find a 0.
    int i;
    for (i = 0; ((bitfield_block >> i) & 1) != 0; i++)
    {
    // If the pos is higher than our bitfield, return no space left.
    if (i >= 63)
        return (void*) error;
    }

    // Set bitfield to show the block as occupied for future searches.
    bitfield_block |= 1L << i;
    
    // Return address of 1024 byte (kilobyte) block.
    return (void*) ALLOC_OFFSET(START_OF_MEMBLOCK, i);
}

void kfree(void* ptr)
{
    // if (((uint64_t)ptr % 1024) != 0)
    //     return;
    // Find pos offset value via BLOCK_OFFSET, then clear bit.
    int i = BLOCK_OFFSET(START_OF_MEMBLOCK, ptr);
    bitfield_block &= ~(1L << i);

    return;
}