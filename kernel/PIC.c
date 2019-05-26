#include "PIC.h"

void remap_PIC(u8 master_offset, u8 slave_offset) {
    u8 master_mask;
    u8 slave_mask;

    // save the masks
    master_mask = inb(PIC_master_mask_port);  
	slave_mask = inb(PIC_slave_mask_port);

    // 0x10 | 0x01 => 0x11  start the ini process in cascade mode
    outb(PIC_master_control_port, 0x11);
    outb(PIC_slave_control_port, 0x11);

    // remap the PIC
    // to move master IRQs 0-7
    // from 0x08 - 0x0F
    // to 0x20 - 0x27
    outb(PIC_master_mask_port, master_offset);

    // remap slave IRQs 8-15
    // from 0x70 - 0x77
    // to 0x28 - 0x2F
    outb(PIC_slave_mask_port, slave_offset);

    // ICW3: tell master that there is slave at IRQ2 (000 0100)
    outb(PIC_master_mask_port, 0x04);
    // ICW3: tell slave PIC it is cascade identity (0000 0010)
    outb(PIC_slave_mask_port, 0x02);
    // ICW4: 8006/88 (MCS-80/85) mode
    outb(PIC_master_mask_port, 0x01);
    outb(PIC_slave_mask_port, 0x01);

    // restore the masks
    outb(PIC_master_mask_port, master_mask);
    outb(PIC_slave_mask_port, slave_mask);
}