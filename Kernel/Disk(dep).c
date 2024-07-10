//! Deprecated from previous M-Mode kernel. WIP

// #include "Disk.h"

// static struct disk
// {
//     struct virtq_desc   *desc;
//     struct virtq_avail  *avail;
//     struct virtq_used   *used;
// } disk;

// void init_disk()
// {
//     // Create new variable for DISK_MMIO_STATUS, set to 0 for reset
//     uint32_t disk_status = 0;

//     //* If device values mismatch, panic.
//     if (
//     (*DISK_MMIO(DISK_MMIO_MAGIC) != 0x74726976) ||
//     (*DISK_MMIO(DISK_MMIO_VERSION) != 2) ||
//     (*DISK_MMIO(DISK_MMIO_DEVICE_ID) != 2) ||
//     (*DISK_MMIO(DISK_MMIO_VENDOR_ID) != 0x554D4551))
//     {
//         kpanic(0xDEAD0000D15C);
//     }

//     // Device reset
//     *DISK_MMIO(DISK_MMIO_STATUS) = disk_status;

//     // Device acknowledge (acknowledge the device as working)
//     disk_status |= DISK_ACKNOWLEDGE;
//     *DISK_MMIO(DISK_MMIO_STATUS) = disk_status;

//     // Device driver (acknowledge that we understand how to drive said device)
//     disk_status |= DISK_DRIVER;
//     *DISK_MMIO(DISK_MMIO_STATUS) = disk_status;

//     // Siphon features from MMIO register, then disable unwanted features.
//     //* Note: Device features is read only, Driver features is write only.
//     uint64_t features = *DISK_MMIO(DISK_MMIO_DEV_FEATURES);
//     // Clear individual feature bits that we dont support
//     features &= ~(1 << DISK_FEAT_RO);
//     features &= ~(1 << DISK_FEAT_SCSI);
//     features &= ~(1 << DISK_FEAT_WCE);
//     features &= ~(1 << DISK_FEAT_MQ);
//     features &= ~(1 << DISK_FEAT_ANY_LAY);
//     features &= ~(1 << DISK_FEAT_EVENT_IDX);
//     features &= ~(1 << DISK_FEAT_INDIRECT);
//     //! Add more features to negate here if needed/convenient
//     *DISK_MMIO(DISK_MMIO_DRV_FEATURES) = features;

//     // Tell device that feature negotiations are done.
//     disk_status |= DISK_FEATURES_OK;
//     *DISK_MMIO(DISK_MMIO_STATUS) = disk_status;

//     // Check to see if device accepts feature negotiations, if not, panic.
//     disk_status = *DISK_MMIO(DISK_MMIO_STATUS);
//     if (!(disk_status & DISK_FEATURES_OK))
//         kpanic(0xDEAD0000D15C);

//     // Initialize queue 0, if already in use, panic.
//     *DISK_MMIO(DISK_MMIO_QUEUE_SEL) = 0;
//     if (*DISK_MMIO(DISK_MMIO_QUEUE_READY))
//         kpanic(0xDEAD0000D15C);

//     // Check queue max size. If 0, queue does not exist, panic.
//     uint32_t disk_max_queue = *DISK_MMIO(DISK_MMIO_QUEUE_MAX);
//     if (disk_max_queue == 0)
//         kpanic(0xDEAD0000D15C);

//     // Allocate and zero these descriptors
//     disk.desc = kalloc();
//     disk.avail = kalloc();
//     disk.used = kalloc();
//     if (disk.desc == NULL || disk.avail == NULL || disk.used == NULL)
//         kpanic(0xDEAD0000D15C);
//     memset(disk.desc, 0, 4096);
//     memset(disk.avail, 0, 4096);
//     memset(disk.used, 0, 4096);

//     // Set queue size to 4 (minimum 3 [probably?] + power of 2)
//     *DISK_MMIO(DISK_MMIO_QUEUE_NUM) = 4;

//     // Set Disk address registers to allocated ptr.
//     *DISK_MMIO(DISK_MMIO_Q_LOW) = (uint64_t)disk.desc;
//     *DISK_MMIO(DISK_MMIO_Q_HIGH) = (uint64_t)disk.desc >> 32;
//     *DISK_MMIO(DISK_MMIO_DRV_LOW) = (uint64_t)disk.avail;
//     *DISK_MMIO(DISK_MMIO_DRV_HIGH) = (uint64_t)disk.avail >> 32;
//     *DISK_MMIO(DISK_MMIO_DEV_LOW) = (uint64_t)disk.used;
//     *DISK_MMIO(DISK_MMIO_DEV_HIGH) = (uint64_t)disk.used >> 32;

//     // Declare disk queue ready
//     *DISK_MMIO(DISK_MMIO_QUEUE_READY) = 0x01;

//     // Declare device ready.
//     disk_status |= DISK_DRIVER_OK;
//     *DISK_MMIO(DISK_MMIO_STATUS) = disk_status;

//     return;
// }

// void disk_access(uint8_t* buffer, uint64_t sector, uint32_t op_type)
// {
//     // If op_type is invalid, panic.
//     if (op_type != DISK_READ && op_type != DISK_WRITE)
//         kpanic(0xDEAD0000D15C);

//     // Set op_status to an obvious erroneous value
//     uint8_t op_status = 0xff;

//     // Create structure disk request
//     struct disk_blk_req disk_req = 
//     {
//         .type = op_type,
//         .reserved = 0,
//         .sector = sector  //^ Sectors are 512-byte sectors that we can load in blocks
//     };
    
//     // Set the header descriptor (command to send to device)
//     disk.desc[0].addr = (uint64_t) &disk_req;           // Address of request struct
//     disk.desc[0].len = sizeof(struct disk_blk_req);     // Size of request struct
//     disk.desc[0].flags = DESC_FLAG_NEXT;                // Flag indicating another descriptor
//     disk.desc[0].next = 1;                              // Descriptor number (disk.desc[x])

//     // Set the buffer descriptor (read or write) for the disk access command
//     disk.desc[1].addr = (uint64_t) buffer;              // Address of buffer to read/write
//     //^ desc[1].len is the issue here
//     disk.desc[1].len = 512;                         // Size of data to read or size of buffer to write
//     if (op_type == DISK_WRITE)                          // Flag 0 for DISK_WRITE (device READS buffer) and 2 for DISK_READ (device WRITES buffer)
//         disk.desc[1].flags = 0;
//     else
//         disk.desc[1].flags = 2;
//     disk.desc[1].flags |= DESC_FLAG_NEXT;               // Flag indicating another descriptor
//     disk.desc[1].next = 2;                              // Descriptor number (disk.desc[x])

//     // Set the status update descriptor for the disk access command
//     disk.desc[2].addr = (uint64_t) &op_status;          // Address of status byte to update upon completion
//     disk.desc[2].len = 1;                               // Length of status byte (1 byte)
//     disk.desc[2].flags = 2;                             // Flag for device write (to status byte)
//     disk.desc[2].next = 0;                              // Since there is no DESC_FLAG_NEXT bit set, we clear desc[2].next

//     //! This till next //! comment is code that I suspect to be broken, but hasnt broke yet.
//     disk.avail->ring[disk.avail->idx % 4] = 0;
//     __sync_synchronize();
//     disk.avail->idx += 1;
//     __sync_synchronize();
//     //! END OF ERROR PRONE CODE

//     // Notify device of which queue is ready (queue 0)
//     *DISK_MMIO(DISK_MMIO_QUEUE_NOTIFY) = 0;

//     // Wait for op_status to updates
//     while (op_status != 0)
//     {
//         // If op_status reports a disk failure, report special kpanic code
//         if (op_status == 1)
//             kpanic(0xDEAD0FA0D15C);
//     }

//     // Reset disk.desc[0..2] to prevent erroneous values
//     for (int i = 0; i < 3; i++)
//     {
//         disk.desc[i].addr = 0L;
//         disk.desc[i].len = 0L;
//         disk.desc[i].flags = 0L;
//         disk.desc[i].next = 0L;
//     }

//     return;
// }

// void disk_read(uint8_t* buffer, uint64_t size, uint64_t disk_pos)
// {
//     uint64_t sector = disk_pos >> 9;
//     uint64_t pos = disk_pos % 512;
//     uint64_t i = 0;
//     uint8_t buftemp[512];

//     //* Pre-loop, read and align to sector
//     disk_access(buftemp, sector++, DISK_READ);
//     for (int j = pos; (j < 512) && (j < pos+size); j++)
//         buffer[i++] = buftemp[j];

//     //* Loop, read 512-bytes and write to buffer
//     // If what is left to write (sector aligned) is greater than a sector.
//     if (((size-pos) > 512) && (size > pos))
//     {
//         // Write directly to buffer at correct offsets for all but the last sector write.
//         for (; i < (size-pos-512); i+=512)
//             disk_access(&buffer[i], sector++, DISK_READ);
//     }

//     //* Post-loop write final x bytes to buffer
//     disk_access(buftemp, sector, DISK_READ);
//     for (int j = 0; i < size; i++, j++)
//         buffer[i] = buftemp[j];
    
//     return;
// }

// void disk_write(const uint8_t* buffer, uint64_t size, uint64_t disk_pos)
// {
//     uint64_t sector = disk_pos >> 9;
//     uint64_t pos = disk_pos % 512;
//     uint64_t i = 0;
//     uint8_t buftemp[512];

//     //* Pre-loop, read, then overwrite.
//     disk_access(buftemp, sector, DISK_READ);
//     for (int j = pos; (j < 512) && (j < pos+size); j++)
//         buftemp[j] = buffer[i++];
//     disk_access(buftemp, sector++, DISK_WRITE);
    
//     //* Loop, write 512-bytes to disk.
//     // If what is left to write (sector aligned) is greater than a sector.
//     if (((size-pos) > 512) && (size > pos))
//     {
//         // Write directly to disk at correct offsets for all but the last sector write.
//         for (; i < (size-pos-512); i+=512)
//             disk_access((uint8_t*) &buffer[i], sector++, DISK_WRITE);
//     }

//     //* Post-loop read, then overwrite final x bytes to buftemp, leaving the rest alone. (Preserves bytes)
//     disk_access(buftemp, sector, DISK_READ);
//     for (int j = 0; i < size; i++, j++)
//         buftemp[j] = buffer[i];
//     disk_access(buftemp, sector, DISK_WRITE);
    
//     return;
// }