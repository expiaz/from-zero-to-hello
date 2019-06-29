#include "PIC.h"

void PIC_remap(u8 master_offset, u8 slave_offset) {
    u8 master_mask; // 10111000 0xb8
    u8 slave_mask; // 10001110 0x8e

    // save the masks
    master_mask = inb(REG_PIC1_DATA);  
	slave_mask = inb(REG_PIC2_DATA);

    // 0x10 | 0x01 => 0x11  start the ini process in cascade mode
    outb(REG_PIC1_CTRL, 0x11);
    outb(REG_PIC2_CTRL, 0x11);

    // remap the PIC
    // to move master IRQs 0-7
    // from 0x08 - 0x0F
    // to 0x20 - 0x27
    outb(REG_PIC1_DATA, master_offset);

    // remap slave IRQs 8-15
    // from 0x70 - 0x77
    // to 0x28 - 0x2F
    outb(REG_PIC2_DATA, slave_offset);

    // ICW3: tell master that there is slave at IRQ2 (000 0100)
    outb(REG_PIC1_DATA, 0x04);
    // ICW3: tell slave PIC it is cascade identity (0000 0010)
    outb(REG_PIC2_DATA, 0x02);
    // ICW4: 8006/88 (MCS-80/85) mode
    outb(REG_PIC1_DATA, 0x01);
    outb(REG_PIC2_DATA, 0x01);

    // restore the masks
    outb(REG_PIC1_DATA, master_mask);
    outb(REG_PIC2_DATA, slave_mask);
}