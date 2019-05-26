#ifndef IDT_H
#define IDT_H

#include "common.h"
#include "isr.h"
#include "PIC.h"

typedef struct {
    /**
     * address of the handler 0:15
     */
    u16 base_low;
    /**
     * code segment that hosts the handler
     */
    u16 segment_selector;
    u8 always0;
    /**
     * |P|DPL|0|110
     * P 1 bit : Interrupt is present
     * DPL 2 bits : ring level of caller
     * 0 1 bit : set to 0 for interrupt gates
     * 110 3 bits: decimal 14 = 32 bit interrupt gate
     */
    u8 flags;
    /**
     * address of the handler 15:31
     */
    u16 base_high;
} __attribute__((packed)) idt_entry_t;

/**
 * points to the IDT, for LIDT instruction
 */
typedef struct {
    /**
     * size of IDT
     */
    u16 limit;
    /**
     * address of IDT
     */
    u32 base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256 // 256 interrupts exists, we'll only define 32
                        // (in which 18 are mapped)
                        // this is sufficient for the CPU
                        // to not crash from NULL handlers

#define KERNEL_CS 0x08
#define IDT_FLAGS 0x8E // 1(P)00(ring 0)0(interrupt)110(32 bits)

// the IDT
idt_entry_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt();

#endif // IDT_H