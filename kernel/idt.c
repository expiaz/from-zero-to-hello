
#include "idt.h"

void set_idt_entry(int isr_number, u32 handler) {
    idt[isr_number].base_low = handler & 0xFFFF;
    idt[isr_number].segment_selector = KERNEL_CS;
    idt[isr_number].always0 = 0;
    idt[isr_number].flags = IDT_FLAGS;
    idt[isr_number].base_high = (handler >> 16) & 0xFFFF;
}

void set_idt() {
    idt_reg.base = (u32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_entry_t) - 1;

    memset((char *) &idt, 0, IDT_ENTRIES * sizeof(idt_entry_t));

    set_idt_entry(0, (u32)isr0);
    set_idt_entry(1, (u32)isr1);
    set_idt_entry(2, (u32)isr2);
    set_idt_entry(3, (u32)isr3);
    set_idt_entry(4, (u32)isr4);
    set_idt_entry(5, (u32)isr5);
    set_idt_entry(6, (u32)isr6);
    set_idt_entry(7, (u32)isr7);
    set_idt_entry(8, (u32)isr8);
    set_idt_entry(9, (u32)isr9);
    set_idt_entry(10, (u32)isr10);
    set_idt_entry(11, (u32)isr11);
    set_idt_entry(12, (u32)isr12);
    set_idt_entry(13, (u32)isr13);
    set_idt_entry(14, (u32)isr14);
    set_idt_entry(15, (u32)isr15);
    set_idt_entry(16, (u32)isr16);
    set_idt_entry(17, (u32)isr17);
    set_idt_entry(18, (u32)isr18);
    set_idt_entry(19, (u32)isr19);
    set_idt_entry(20, (u32)isr20);
    set_idt_entry(21, (u32)isr21);
    set_idt_entry(22, (u32)isr22);
    set_idt_entry(23, (u32)isr23);
    set_idt_entry(24, (u32)isr24);
    set_idt_entry(25, (u32)isr25);
    set_idt_entry(26, (u32)isr26);
    set_idt_entry(27, (u32)isr27);
    set_idt_entry(28, (u32)isr28);
    set_idt_entry(29, (u32)isr29);
    set_idt_entry(30, (u32)isr30);
    set_idt_entry(31, (u32)isr31);

    set_isr();

    __asm__("lidt (%%eax)" : : "a" (&idt_reg));
}