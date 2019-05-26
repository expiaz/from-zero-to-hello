#ifndef PIC_H
#define PIC_H

#include "../kernel/common.h"

#define PIC1		    0x20		/* IO base address for master PIC */
#define PIC2		    0xA0		/* IO base address for slave PIC */

#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)

#define PIC_EOI         0x20

void PIC_remap(u8 master_offset, u8 slave_offset);

#endif // PIC_H