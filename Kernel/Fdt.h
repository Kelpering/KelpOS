#ifndef __FDT_H__
#define __FDT_H__

#include "Klib.h"

typedef struct 
{
    uint64_t syscon_addr;
    uint64_t syscon_size;
    uint16_t shutdown_val;
    uint64_t shutdown_offset;
    uint16_t reset_val;
    uint64_t reset_offset;

    uint64_t plic_addr;
    uint64_t plic_size;

    uint64_t uart_addr;
    uint64_t uart_size;
    uint8_t  uart_irq;
} fdt_struct;

// Share global fdt_list (Found in Fdt.c) with any file including Fdt.h
extern fdt_struct fdt_list;

/// @brief Initializes the global fdt_list struct for use in 
void init_fdt();

/// @brief Shuts down the cpu and associated devices. Does not clean or save anything.
void shutdown() __attribute__((noreturn));

/// @brief Resets the cpu and associated devices. Does not clean or save anything.
void reset() __attribute__((noreturn));

#endif // __FDT_H__


//! WIP code for Flattened Device Tree parsing. Use as reference when revisiting.
/////* Const

// // Big endian converted
// // #define FDT_MAGIC REVERSE_32(0xd00dfeed)
// // #define FDT_BEGIN REVERSE_32(0x1)
// // #define FDT_END_NODE REVERSE_32(0x2)
// // #define FDT_PROP REVERSE_32(0x3)
// // #define FDT_NOP REVERSE_32(0x04)
// // #define FDT_END REVERSE_32(0x9)

/////* Structs

// // typedef struct {
// //     uint32_t magic;             // 0xd00dfeed in big endian format
// //     uint32_t totalsize;         // Total size of FDT struct (including free space gaps) in bytes
// //     uint32_t off_dt_struct;     // Offset (in bytes) of the struct block
// //     uint32_t off_dt_strings;    // Offset (in bytes) of the string block
// //     uint32_t off_mem_rsvmap;    // Offset (in bytes) of the memres block
// //     uint32_t version;           // Version of the FDT/DTB structure
// //     uint32_t last_comp_version; // Lowest compatible version of the FDT/DTB structure
// //     uint32_t boot_cpuid_phys;   // Physical ID of the boot cpu
// //     uint32_t size_dt_strings;   // Length (in bytes) of the strings block
// //     uint32_t size_dt_struct;    // Length (in bytes) of the struct block
// // } __attribute__((packed)) fdt_header;

// // // Mem Res (rename comment)
// // typedef struct {
// //     uint64_t address;
// //     uint64_t size;
// // } __attribute__((packed)) fdt_reserved_entry;

// // // Struct (rename comment)
// // typedef struct {
// //     uint32_t token; // 0x03 (Big-Endian)
// //     uint32_t length;
// //     uint32_t name_offset;
// // } __attribute__((packed)) fdt_prop;


// // int fdt_init(fdt_header *fdt, parsed_device_list *device_list);

// // fdt_prop *fdt_next_prop(const fdt_prop *CurrProp);

// // char *fdt_prop_name(fdt_prop *Prop, fdt_header *fdt);

// // // Parses FDT and returns useful values into fdt_parsed_list struct. Expanded per device supported.
// // void *parse_compatible(char *node_str, fdt_header *fdt);