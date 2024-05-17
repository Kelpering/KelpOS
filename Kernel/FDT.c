#include "FDT.h"

int fdt_parse(fdt_header *fdt, fdt_parsed_list *fdt_parsed)
{
    // If fdt magic is invalid, return error
    if (fdt->magic != REVERSE_32(FDT_MAGIC))
        return error;
    
    //* Assume FDT is sane after this point, add checks if desired before here.

    uint32_t *power_node = parse_compatible("syscon-poweroff", fdt);
    uint32_t *reboot_node = parse_compatible("syscon-reboot", fdt);


    // Parse FDT for poweroff and reboot
        // These nodes have a phandle ref to test (sifive)
        // Detect this node, account for offsets, set fdt_parsed to register addr (w/ offset) and value to write
    // Parse FDT for UART
        // Add info here when required
        // Just detect the device, no setup
    // Parse FDT for VGA
        // Add info here when required
    // Parse FDT for 
    

    // Functions to be useful:
        //^ detect_compatible (Detects a string that is compatible, returns node ptr? NULL if none?)
        //^ detect_phandle (If called, find referenced node by phandle. Use values for further use.)
        //^ parse_addr (Parse a nodes base and size, return struct {ptr, size})

    return 1;

}

void *parse_compatible(char *node_str, fdt_header *fdt)
{
    uint32_t *StructBlock = ((uint8_t*) fdt)+REVERSE_32(fdt->off_dt_struct);

    // Probably some sort of error interrupt being thrown here. Fix this issue, then actually add an interrupt panic.
    uint32_t TEST = REVERSE_32(*StructBlock);
    char *Node_Name = ((uint8_t*) StructBlock)+4;
    char First = *Node_Name;

    // Search root nodes until you either reach the end of the nodes or find a matching string.

    //^ Search a node, then go to the next
    //^ Find the compatible section of the string
    //^ Check null terminated string (there might be multiple strings)
    return (void*) 0;
}

//! Next node will take a current byte ptr (uint8_t), then find the next node, or return NULL if FDT_END is found
//! Next prop will take a current fdt_prop struct, then find the next prop within the node, or return NULL if FDT_END_NODE is found.
//! fdt_string_offset will convert an fdt_prop struct name offset into a char* to the actual NULL terminated string in the string offset block.
//! Build these core features first, then work out into necessary algorithms. (Search for string, Search compatible, etc)

fdt_prop *fdt_next_prop(fdt_prop *Current)
{

}

void *fdt_next_node(void *Current, fdt_header *fdt)
{
    // Take current node (Dont assume first one is passed here)
    // Find the next node in the structure, skipping all the other bytes and structs until you reach an entirely new node.
    // If the last node is found, (FDT_END) return NULL pointer, which will be detected in search.
}

char *fdt_string_offset(uint32_t offset, fdt_header *fdt)
{
    char *StringBlock = ((uint8_t*) fdt) + REVERSE_32(fdt->off_dt_strings);

    return (StringBlock+offset);
}