#include "FDT.h"

int fdt_parse(fdt_header *fdt, fdt_parsed_list *fdt_parsed)
{
    // If fdt magic is invalid, return error
    if (fdt->magic != FDT_MAGIC)
        return erorr
    
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

}

void* parse_compatible(char* node_str, fdt_header *fdt)
{
    // Search root nodes until you either reach the end of the nodes or find a matching string.

    //^ Search a node, then go to the next
    //^ Find the compatible section of the string
    //^ Check null terminated string (there might be multiple strings)
}