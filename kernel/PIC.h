#ifndef PIC_H
#define PIC_H

#include "common.h"

#define PIC_master_control_port   0x20
#define PIC_master_mask_port      0x21

#define PIC_slave_control_port    0xA0
#define PIC_slave_mask_port       0xA1

#define PIC_EOI             0x20

void remap_PIC(u8 master_offset, u8 slave_offset);

#endif // PIC_H