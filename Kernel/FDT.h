#ifndef _FDT_H_
#define _FDT_H_

#include "KLib.h"

//* Const

// Big endian converted
#define FDT_MAGIC 0xd00dfeed
#define FDT_BEGIN 0x1
#define FDT_END_NODE 0x2
#define FDT_PROP 0x3
#define FDT_END 0x9

//* Structs

typedef struct {
    uint32_t magic;             // 0xd00dfeed in big endian format
    uint32_t totalsize;         // Total size of FDT struct (including free space gaps) in bytes
    uint32_t off_dt_struct;     // Offset (in bytes) of the struct block
    uint32_t off_dt_strings;    // Offset (in bytes) of the string block
    uint32_t off_mem_rsvmap;    // Offset (in bytes) of the memres block
    uint32_t version;           // Version of the FDT/DTB structure
    uint32_t last_comp_version; // Lowest compatible version of the FDT/DTB structure
    uint32_t boot_cpuid_phys;   // Physical ID of the boot cpu
    uint32_t size_dt_strings;   // Length (in bytes) of the strings block
    uint32_t size_dt_struct;    // Length (in bytes) of the struct block
} __attribute__((packed)) fdt_header;

// Mem Res (rename comment)
typedef struct {
    uint64_t address;
    uint64_t size;
} __attribute__((packed)) fdt_reserved_entry;

// Struct (rename comment)
typedef struct {
    uint32_t length;
    uint32_t name_offset;
} __attribute__((packed)) fdt_prop;


// Struct with values that are useful to core device drivers. Parsed from firmware provided FDT struct by fdt_parse().
typedef struct {
    // Handle Power/Reboot systems
    uint32_t *power_addr;
    uint32_t power_value;
    uint32_t *reboot_addr;
    uint32_t reboot_value;

    // Handle UART
    void *uart_addr;

    // Handle VGA
    void *vga_addr;

    // Handle ram (largest block of contiguous ram)
    void *ram_start;
    void *ram_end;

} fdt_parsed_list;


int fdt_parse(fdt_header *fdt, fdt_parsed_list *fdt_parsed);

// Parses FDT and returns useful values into fdt_parsed_list struct. Expanded per device supported.
void *parse_compatible(char *node_str, fdt_header *fdt);

#endif // _FDT_H_