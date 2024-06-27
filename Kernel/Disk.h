#ifndef _DISK_H_
#define _DISK_H_

#include "KLib.h"
#include "Memory.h"

#define DISK_READ   0
#define DISK_WRITE  1


#define DISK_MMIO(x) ((volatile uint32_t*)(0x10001000L + x))

// To be used in DISK_MMIO for specific MMIO register locations
#define DISK_MMIO_MAGIC         0x000   // 0x74726976
#define DISK_MMIO_VERSION       0x004
#define DISK_MMIO_DEVICE_ID     0x008
#define DISK_MMIO_VENDOR_ID     0x00C
#define DISK_MMIO_DEV_FEATURES  0x010
#define DISK_MMIO_DRV_FEATURES  0x020
#define DISK_MMIO_QUEUE_SEL     0x030
#define DISK_MMIO_QUEUE_MAX     0x034
#define DISK_MMIO_QUEUE_NUM     0x038
#define DISK_MMIO_QUEUE_READY   0x044
#define DISK_MMIO_QUEUE_NOTIFY  0x050
#define DISK_MMIO_STATUS        0x070
#define DISK_MMIO_Q_LOW	        0x080 
#define DISK_MMIO_Q_HIGH	    0x084
#define DISK_MMIO_DRV_LOW	    0x090
#define DISK_MMIO_DRV_HIGH	    0x094
#define DISK_MMIO_DEV_LOW	    0x0a0
#define DISK_MMIO_DEV_HIGH	    0x0a4

// To be used with DISK_MMIO(DISK_MMIO_STATUS) register, specific bits.
#define DISK_ACKNOWLEDGE    (1<<0)
#define DISK_DRIVER         (1<<1)
#define DISK_DRIVER_OK      (1<<2)
#define DISK_FEATURES_OK    (1<<3)

// To be used with DISK_MMIO(DISK_MMIO_~~~_FEATURES) register, bit positions.
#define DISK_FEAT_RO        5
#define DISK_FEAT_SCSI      7
#define DISK_FEAT_WCE       11
#define DISK_FEAT_MQ        12
#define DISK_FEAT_ANY_LAY   27
#define DISK_FEAT_INDIRECT  28
#define DISK_FEAT_EVENT_IDX 29

// To be used with disk descriptor (virtq_desc) struct
#define DESC_FLAG_NEXT  1 // chained with another descriptor
#define DESC_FLAG_WRITE 2 // device writes (vs read)

struct virtq_desc
{
    uint64_t addr;
    uint32_t len;
    uint16_t flags;
    uint16_t next;
};

struct virtq_avail
{
    uint16_t flags;
    uint16_t idx;
    uint16_t ring[4];
    uint16_t unused;
};

struct virtq_used_elem
{
    uint32_t id;
    uint32_t len;
};

struct virtq_used
{
    uint16_t flags;
    uint16_t idx;
    struct virtq_used_elem ring[4];
};

struct disk_blk_req
{
    uint32_t type;
    uint32_t reserved;
    uint64_t sector;
};

void init_disk();
void disk_access(uint8_t* buffer, uint64_t bufsize, uint64_t disk_addr, uint32_t op_type);

#endif // _DISK_H_