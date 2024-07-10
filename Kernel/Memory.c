#include "Memory.h"

static free_pg *free_page_elem;

void init_memory()
{
    // Free start of mem after kernel (page aligned) till end of memory (statically chosen)

    // Create doubly linked circular list (first and last are incorrect)
    for (uint64_t pg = (uint64_t) START_OF_MEM; pg < (uint64_t) END_OF_MEM; pg+=PG_SIZE)
    {
        ((free_pg*) pg)->prev = (free_pg*)(pg-PG_SIZE);
        ((free_pg*) pg)->next = (free_pg*)(pg+PG_SIZE);
    }

    // Fix first and last
    ((free_pg*) START_OF_MEM)->prev = (free_pg*) ((uint64_t)END_OF_MEM-PG_SIZE);
    ((free_pg*) ((uint64_t)END_OF_MEM-PG_SIZE))->next = (free_pg*) START_OF_MEM;

    // Set the current free page to be the first element.
    free_page_elem = START_OF_MEM;

    // Return a pointer to the global free_page_elem (for debugging).
    return;
}

void *kalloc()
{
    // If there are no free pages left, return NULL.
    if (free_page_elem == NULL)
        return NULL;

    // Make the previous page point to the next page.
    (free_page_elem->prev)->next = free_page_elem->next;
    // Make the next page point to the previous page.
    (free_page_elem->next)->prev = free_page_elem->prev;

    // Save the page we are allocating and replace the (now allocated) free_page_elem.
    void *temp_page_elem = free_page_elem;
    free_page_elem = free_page_elem->next;

    return temp_page_elem;
}

void *zalloc()
{
    // Allocate a page and zero the memory.
    void *page = kalloc();
    memset(page, 0, PG_SIZE);

    return page;
}

void kfree(void *page)
{
    // If the page is an invalid ptr, panic.
    if ((uint64_t)page < (uint64_t)START_OF_MEM || (uint64_t)page > (uint64_t)END_OF_MEM || ((uint64_t)page % PG_SIZE) != 0)
        uart_panic("KFREE", "Invalid PhysMem Page\nPage received: %P", page);


    // Convert page into a free page (between free_page_elem and free_page_elem->next)
    ((free_pg*)page)->prev = free_page_elem;
    ((free_pg*)page)->next = free_page_elem->next;

    // Insert page directly after free_page_elem
    free_page_elem->next->prev = page;
    free_page_elem->next = page;

    return;
}