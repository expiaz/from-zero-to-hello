
#include "IDT.h"

void IDT_set_entry(int isr_number, u32 handler) {
    idt[isr_number].base_low = handler & 0xFFFF;
    idt[isr_number].segment_selector = KERNEL_CS;
    idt[isr_number].always0 = 0;
    idt[isr_number].flags = IDT_FLAGS;
    idt[isr_number].base_high = (handler >> 16) & 0xFFFF;
}

void IDT_init() {
    idt_reg.base = (u32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_entry_t) - 1;

    memset((u8 *) &idt, 0, IDT_ENTRIES * sizeof(idt_entry_t));

    // CPU fault handlers
    IDT_set_entry(0, (u32)isr0);
    IDT_set_entry(1, (u32)isr1);
    IDT_set_entry(2, (u32)isr2);
    IDT_set_entry(3, (u32)isr3);
    IDT_set_entry(4, (u32)isr4);
    IDT_set_entry(5, (u32)isr5);
    IDT_set_entry(6, (u32)isr6);
    IDT_set_entry(7, (u32)isr7);
    IDT_set_entry(8, (u32)isr8);
    IDT_set_entry(9, (u32)isr9);
    IDT_set_entry(10, (u32)isr10);
    IDT_set_entry(11, (u32)isr11);
    IDT_set_entry(12, (u32)isr12);
    IDT_set_entry(13, (u32)isr13);
    IDT_set_entry(14, (u32)isr14);
    IDT_set_entry(15, (u32)isr15);
    IDT_set_entry(16, (u32)isr16);
    IDT_set_entry(17, (u32)isr17);
    IDT_set_entry(18, (u32)isr18);
    IDT_set_entry(19, (u32)isr19);
    IDT_set_entry(20, (u32)isr20);
    IDT_set_entry(21, (u32)isr21);
    IDT_set_entry(22, (u32)isr22);
    IDT_set_entry(23, (u32)isr23);
    IDT_set_entry(24, (u32)isr24);
    IDT_set_entry(25, (u32)isr25);
    IDT_set_entry(26, (u32)isr26);
    IDT_set_entry(27, (u32)isr27);
    IDT_set_entry(28, (u32)isr28);
    IDT_set_entry(29, (u32)isr29);
    IDT_set_entry(30, (u32)isr30);
    IDT_set_entry(31, (u32)isr31);

    // setup exceptions messages for CPU faults
    // bc global init won't work with compiler
    ISR_init();

    // remap the PIC
    // move master IRQs 0-7
    // from 0x08 - 0x0F
    // to 0x20 - 0x27
    // remap slave IRQs 8-15
    // from 0x70 - 0x77
    // to 0x28 - 0x2F
    PIC_remap(0x20, 0x28);

    // master PIC
    IDT_set_entry(32, (u32)irq0);
    IDT_set_entry(33, (u32)irq1);
    IDT_set_entry(34, (u32)irq2);
    IDT_set_entry(35, (u32)irq3);
    IDT_set_entry(36, (u32)irq4);
    IDT_set_entry(37, (u32)irq5);
    IDT_set_entry(38, (u32)irq6);
    IDT_set_entry(39, (u32)irq7);
    // slave PIC
    IDT_set_entry(40, (u32)irq8);
    IDT_set_entry(41, (u32)irq9);
    IDT_set_entry(42, (u32)irq10);
    IDT_set_entry(43, (u32)irq11);
    IDT_set_entry(44, (u32)irq12);
    IDT_set_entry(45, (u32)irq13);
    IDT_set_entry(46, (u32)irq14);
    IDT_set_entry(47, (u32)irq15);

    __asm__("lidt (%%eax)" : : "a" (&idt_reg));
    // we must reenable interrupt now that we have set up
    // our IDT in protected mode
    __asm__("sti");
}