#include "FDT.h"

#define SHUTDOWN_ADDR   0x100000
#define SHUTDOWN_VAL    0x5555
#define RESET_ADDR      0x100000
#define RESET_VAL       0x7777

// These are volatile to allow for writes to happen immediately
void shutdown()
{
    while (true)
        *((volatile uint32_t*) SHUTDOWN_ADDR) = SHUTDOWN_VAL;
    kpanic(0xDEAD);
}

void reset()
{
    while (true)
        *((volatile uint32_t*) RESET_ADDR) = RESET_VAL;
    kpanic(0xDEAD);
}

// // void init_plic()
// // {
// //     //^ Init the plic itself
// //     return;
// // }

void init_plic_device(uint32_t irq)
{
    *(uint32_t*)(PLIC_ADDR + irq*4) = 1;
    *(uint32_t*)(PLIC_ADDR + 0x2000) = 1 << irq;
    
    //^ Initializes a specific device for the plic
    //^ Write a default value for priority
    return;
}

uint32_t claim_plic()
{   
    uint32_t irq = *(uint32_t*)(PLIC_ADDR+0x200004);
    return irq;
}

void complete_plic(uint32_t irq)
{
    *(uint32_t*)(PLIC_ADDR+0x200004) = irq;
    return;
}



//? Revisit later to discover constants, not assume.

// // #define POWEROFF_ADDR 0x100000
// // #define REBOOT_ADDR 0x100000
// // #define POWEROFF_COMM 0x5555
// // #define REBOOT_COMM 0x7777
// // #define UART_ADDR 0x100;

// // int fdt_init(fdt_header *fdt, parsed_device_list *device_list)
// // {
// //     // If fdt magic is invalid, return error
// //     // if (fdt->magic != FDT_MAGIC)
// //     //     return error;

// //     device_list->power_addr = POWEROFF_ADDR;
// //     device_list->power_value = POWEROFF_COMM;
// //     device_list->reboot_addr = REBOOT_ADDR;
// //     device_list->reboot_value = REBOOT_COMM;

// //     device_list->uart_addr = UART_ADDR;
// //     	// 	serial@10000000 {
// // 		// 	interrupts = <0x0a>;
// // 		// 	interrupt-parent = <0x03>;
// // 		// 	clock-frequency = "\08@";
// // 		// 	reg = <0x00 0x10000000 0x00 0x100>;
// // 		// 	compatible = "ns16550a";
// // 		// };



    
// //     //* Assume FDT is sane after this point, add checks if desired before here.

// //     // uint32_t *power_node = parse_compatible("syscon-poweroff", fdt);
// //     // uint32_t *reboot_node = parse_compatible("syscon-reboot", fdt);


// //     // Parse FDT for poweroff and reboot
// //         // These nodes have a phandle ref to test (sifive)
// //         // Detect this node, account for offsets, set fdt_parsed to register addr (w/ offset) and value to write
// //     // Parse FDT for UART
// //         // Add info here when required
// //         // Just detect the device, no setup
// //     // Parse FDT for VGA
// //         // Add info here when required
// //     // Parse FDT for 
    

// //     // Functions to be useful:
// //         //^ detect_compatible (Detects a string that is compatible, returns node ptr? NULL if none?)
// //         //^ detect_phandle (If called, find referenced node by phandle. Use values for further use.)
// //         //^ parse_addr (Parse a nodes base and size, return struct {ptr, size})

// //     return 1;

// // }

// // void *parse_compatible(char *node_str, fdt_header *fdt)
// // {
// //     fdt_prop *StructBlock = ((uint8_t*) fdt)+REVERSE_32(fdt->off_dt_struct);
// //     StructBlock = ((uint8_t*) StructBlock) + 8;

// //     char *Test = fdt_prop_name(StructBlock, fdt);
    
// //     // Probably some sort of error interrupt being thrown here. Fix this issue, then actually add an interrupt panic.
// //     // uint32_t TEST = REVERSE_32(*StructBlock);
// //     // char *Node_Name = ((uint8_t*) StructBlock)+4;
// //     // char First = *Node_Name;

// //     // Search root nodes until you either reach the end of the nodes or find a matching string.

// //     //^ Search a node, then go to the next
// //     //^ Find the compatible section of the string
// //     //^ Check null terminated string (there might be multiple strings)
// //     return (void*) 0;
// // }

// // //! Next node will take a current byte ptr (uint8_t), then find the next node, or return NULL if FDT_END is found
// // //! Next prop will take a current fdt_prop struct, then find the next prop within the node, or return NULL if FDT_END_NODE is found.
// // //! fdt_string_offset will convert an fdt_prop struct name offset into a char* to the actual NULL terminated string in the string offset block.
// // //! Build these core features first, then work out into necessary algorithms. (Search for string, Search compatible, etc)

// // fdt_prop *fdt_next_prop(const fdt_prop *CurrProp)
// // {
// //     // Calculates offset from CurrProp to the next Prop token
// //     uint32_t Offset = 11 + REVERSE_32(CurrProp->length);
// //     Offset += 3 - ((Offset+3) %4);

// //     // Finds NextProp, skips all FDT_NOP tokens.
// //     fdt_prop* NextProp = ((uint8_t*) CurrProp) + Offset;
// //     while(NextProp->token == FDT_NOP)
// //         NextProp = ((uint8_t*) NextProp) + 1;

// //     // Checks if token is FDT_PROP. If not, return NULL to indicate end of properties.
// //     if (NextProp->token != FDT_PROP)
// //         return NULL;
    
// //     // Return NextProp
// //     return NextProp;
// // }

// // void *fdt_next_node(void *Current, fdt_header *fdt)
// // {
// //     // Take current node (Dont assume first one is passed here)
// //     // Find the next node in the structure, skipping all the other bytes and structs until you reach an entirely new node.
// //     // If the last node is found, (FDT_END) return NULL pointer, which will be detected in search.
// // }

// // char *fdt_prop_name(fdt_prop *Prop, fdt_header *fdt)
// // {
// //     char *StringBlock = ((uint8_t*) fdt) + REVERSE_32(fdt->off_dt_strings);

// //     return (StringBlock+REVERSE_32(Prop->name_offset));
// // }