#ifndef PIC_H
#define PIC_H

#include "../kernel/common.h"

#define REG_PIC1 0x20 /* IO base address for master PIC */
#define REG_PIC2 0xA0 /* IO base address for slave PIC */

#define REG_PIC1_CTRL REG_PIC1
#define REG_PIC1_DATA (REG_PIC1 + 1)
#define REG_PIC2_CTRL REG_PIC2
#define REG_PIC2_DATA (REG_PIC2 + 1)

#define PIC_EOI 0x20

void PIC_remap(u8 master_offset, u8 slave_offset);

#endif // PIC_H