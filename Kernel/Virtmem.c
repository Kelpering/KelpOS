#include "Virtmem.h"

// Global kernel pagetable (usermode will not have access to it)
pgtbl kernel_pagetable;

void init_virtmem()
{
    //* Create temporary kpgtbl (kernel Page Table)
    pgtbl kpgtbl = zalloc();
    if (!kpgtbl)
        uart_panic("INIT_VIRTMEM", "zalloc failed");

    //* Map all MMIO devices and Kernel to the pagetable
    int error_code = 0;

    //? Map the kernel code as RX (read/execute) so that malicious or malformed code can't modify the kernel
    error_code = map_pages(kpgtbl, (uint64_t)START_OF_KERNEL, ((uint64_t)END_OF_TEXT-(uint64_t)START_OF_KERNEL), (uint64_t)START_OF_KERNEL, PTE_R | PTE_X);
    if (error_code == error)
        uart_panic("VIRTMEM_INIT", "map pages failed to allocate [KERNEL CODE]");

    //? Map the kernel data sectors which are all after END_OF_TEXT (See Kernel.ld) as RW (read/write) so that malicious code can't run from data.
    error_code = map_pages(kpgtbl, (uint64_t)END_OF_TEXT, ((uint64_t)END_OF_KERNEL-(uint64_t)END_OF_TEXT), (uint64_t)END_OF_TEXT, PTE_R | PTE_W);
    if (error_code == error)
        uart_panic("VIRTMEM_INIT", "map pages failed to allocate [KERNEL DATA]");

    //? Map the kernel dynamic memory (separated for readability) as RW (read/write) so that malicious code can't run from data.
    error_code = map_pages(kpgtbl, (uint64_t)START_OF_MEM, (uint64_t)(END_OF_MEM)-(uint64_t)(START_OF_MEM), (uint64_t)START_OF_MEM, PTE_R | PTE_W);
    if (error_code == error)
        uart_panic("VIRTMEM_INIT", "map pages failed to allocate [KERNEL DATA]");

    //? Map UART
    error_code = map_pages(kpgtbl, fdt_list.uart_addr, fdt_list.uart_size, fdt_list.uart_addr, PTE_R | PTE_W);
    if (error_code == error)
        uart_panic("VIRTMEM_INIT", "map pages failed to allocate [UART]");

    //? Map PLIC
    error_code = map_pages(kpgtbl, fdt_list.plic_addr, fdt_list.plic_size, fdt_list.plic_addr, PTE_R | PTE_W);
    if (error_code == error)
        uart_panic("VIRTMEM_INIT", "map pages failed to allocate [PLIC]");

    //? Map Syscon (allows for system shutdown and reset)
    error_code = map_pages(kpgtbl, fdt_list.syscon_addr, fdt_list.syscon_size, fdt_list.syscon_addr, PTE_R | PTE_W);
    if (error_code == error)
        uart_panic("VIRTMEM_INIT", "map pages failed to allocate [PLIC]");

    //* Enable paging
    // Set global kernel pagetable to be the now correctly configured kpgtbl
    kernel_pagetable = kpgtbl;

    // Finish all writes before this point
    sfence_vma();

    // Write Sv39-Mode to SATP
    // Divide kernel_pagetable addr by 4096 to fit in SATP.PPN
    satp_write((8L<<60) | ((uint64_t)kernel_pagetable>>12));

    // Flush previously cached memory addresses.
    sfence_vma();

    return;
}

pte *walk(pgtbl pagetable, uint64_t virt_addr, int alloc)
{
    if (virt_addr > (1L<<(9+9+9+12-1)))
        uart_panic("WALK", "Virtual Address > Max address\nAddr: %X", virt_addr);

    // Loop until we find the leaf pte
    for (int level = 2; level > 0; level--)
    {
        // Calculate the entry number by referencing the proper va.VPN[level] bits.
        int num = (virt_addr>>(12+(9*level))) & 0x1FF;
        pte *entry = &pagetable[num];

        // If the entry is valid, replace pagetable with the new reference and continue to the next level.
        if (*entry & PTE_V)
            pagetable = (pgtbl) (((*entry) >> 10) << 12); // Clear non-address bits, then multiply by size of page (4096)
        else    // Otherwise, allocate or report no such pte existing (depending on alloc)
        {
            // If alloc denied, quit and return zero.
            if (alloc == 0)
                return 0;
            // Otherwise, allocate a new pagetable and return zero on failure.
            if ((pagetable = zalloc()) == 0)
                return 0;

            // Add newly allocated pagetable as the pte we just accessed.        
            *entry = (((uint64_t)pagetable) >> 12) << 10; // Clear non-address bits, then shift by 10 bits to match PPN in pte.
            *entry |= PTE_V;    // Mark the new entry as valid.
        }
    }

    // pagetable contains the final branch. Index a final time (level 0) to find the pte leaf.
    return &pagetable[(virt_addr>>12) & 0x1FF];
}

int map_pages(pgtbl pagetable, uint64_t pa, size_t size, uint64_t va, int perms)
{
    if (size == 0)
        uart_panic("MAP-PAGES", "size: 0");
    
    // Align va so that the page is allocated to contain the minimum va value.
    uint64_t curr = PAGE_ALIGN_DOWN(va); // first page aligned to contain the minimum va address
    uint64_t last = PAGE_ALIGN_DOWN(va + size - 1); // last page aligned to contain the maximum va address.
    pte *entry; // Contains the current entry being modified

    for (; curr <= last; curr+=PG_SIZE, pa+=PG_SIZE)
    {
        // Entry is a newly allocated leaf in the specified pagetable.
        entry = walk(pagetable, curr, 1);
        if (entry == 0)
            return error;
        
        if (*entry & PTE_V)
            uart_panic("MAP-PAGES", "virt_addr %P has already been mapped.\nentry config: %X", curr, *entry);
        
        // Map the corresponding pa to the va represented by the pagetable entry with the correct permissions.
        *entry = ((pa >> 12) << 10) | perms | PTE_V;
    }

    return 0;
}

void unmap_pages(pgtbl pagetable, uint64_t va, size_t size)
{
    if (size == 0)
        uart_panic("UNMAP-PAGES", "size: %d", size);
    
    // Align va so that the page is allocated to contain the minimum va value.
    uint64_t curr = PAGE_ALIGN_DOWN(va); // first page aligned to contain the minimum va address
    uint64_t last = PAGE_ALIGN_DOWN(va + size - 1); // last page aligned to contain the maximum va address.
    pte *entry; // Contains the current entry being modified

    for (; curr <= last; curr+=PG_SIZE, va+=PG_SIZE)
    {
        // Entry is an existing leaf, or (if unallocated) is ignored.
        entry = walk(pagetable, curr, 0);
        if (entry == 0)
            continue;
        
        // Reset the entry to be 0 (unallocated) which deletes previous virtual address association.
        *entry = (uint64_t) 0;
    }

    return;
}

int map_user_pages(pgtbl pagetable, uint64_t va, size_t size, uint64_t user_va)
{
    // Take both alloc/unalloc pages and convert them into user pages.
    // Return NULL on failure, pointer to beginning of mem region on success
    // va needs to be page-aligned (because we assume this is from kalloc).
    // size in bytes of the user space (aligned up to be page-aligned)

    if (size == 0 || (va % PG_SIZE != 0))
        uart_panic("MAP-USERPAGE", "size: %d\nva: 0x%X", size, va);

    // Unmap all required pages, then map all pages into RWX user pages.
    unmap_pages(pagetable, va, size);
    int error_code = map_pages(pagetable, va, size, user_va, PTE_R | PTE_W | PTE_X | PTE_U);

    return error_code;
}