#ifndef __VIRTMEM_H__
#define __VIRTMEM_H__

#include "Klib.h"
#include "Memory.h"
#include "Fdt.h"
#include "Riscv.h"

// bitwise AND to the inverse of 0xFFF (declared Unsigned Long Long)
#define PAGE_ALIGN_UP(x) ((((uint64_t)x)+4095) & ~(0xFFFULL))
#define PAGE_ALIGN_DOWN(x) (((uint64_t)x) & ~(0xFFFULL))

#define PTE_V (1<<0)    // Set Page Valid
#define PTE_R (1<<1)    // Set Page Readable
#define PTE_W (1<<2)    // Set Page Writable
#define PTE_X (1<<3)    // Set Page Executable
#define PTE_U (1<<4)    // Set Page User only (otherwise Supervisor only)

typedef uint64_t pte;       // A single entry into the pgtbl array.
typedef uint64_t *pgtbl;    // Actually a uint64_t[512].

void init_virtmem();

/// @brief Searches the referenced pagetable to return the matching pte to the provided virt_addr. Allocates new pagetables if desired (alloc).
/// @param pagetable The pagetable to search (and allocate to).
/// @param virt_addr The virtual address to match to a corresponding pte.
/// @param alloc Whether or not to allocate new pagetable branches if needed.
/// @returns Returns 0 if walk failed to find (or allocate) a corresponding pte to virt_addr. Otherwise, returns a pointer to the pte.
pte *walk(pgtbl pagetable, uint64_t virt_addr, int alloc);

    // Return error (-1) on failure, 0 on success.
    // for a given pagetable, map a physical address range from [pa to pa+size] to a virtual address va. Apply permissions from perm
    // pa, size, and va might not be page-aligned. Align them in respect so that the page will contain the address (and data in sizes case)

/// @brief For a given pagetable, map a physical address range (pa->pa+size) to a virtual address va. Configure permissions as desired.
/// @param pagetable Any valid pagetable, will be modified to contain newly allocated entries for the va.
/// @param pa Physical address to begin map from. May not be page-aligned.
/// @param size Size of data to map in bytes, beginning from pa.
/// @param va Virtual address to begin map to. May not be page-aligned.
/// @param perms Permissions to apply to each page allocated (begins with PTE_), PTE_V not required.
/// @return 0 on success, -1 on failure.
int map_pages(pgtbl pagetable, uint64_t pa, size_t size, uint64_t va, int perms);

#endif // __VIRTMEM_H__