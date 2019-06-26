#include "./ISR.h"

char *interrupt_names[256]; /* = {
        "Division by zero exception",
        "Debug exception",
        "Non maskable interrupt",
        "Breakpoint exception",
        "into detected overflow",
        "Out of bounds exception",
        "Invalid opcode exception",
        "No coprocessor exception",
        "Double fault",
        "Coprocessor segment overrun",
        "Bad TSS",
        "Segment not present",
        "Stack fault",
        "General protection fault",
        "Page fault",
        "Unknown interrupt exception",
        "coprocessor fault",
        "Alignment check exception",
        "Machine check exception",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"
    }; */

    
// char *interrupt_names[15];

isr_t interrupt_handlers[256];

void register_interrupt_handler(u8 number, isr_t handler) {
  interrupt_handlers[number] = handler;
}

void ISR_init() {
    memset((u8 *) &interrupt_handlers, 0, 256 * sizeof(isr_t));

    interrupt_names[0] = "Division by zero exception";
    interrupt_names[1] = "Debug exception";
    interrupt_names[2] = "Non maskable interrupt";
    interrupt_names[3] = "Breakpoint exception";
    interrupt_names[4] = "into detected overflow";
    interrupt_names[5] = "Out of bounds exception";
    interrupt_names[6] = "Invalid opcode exception";
    interrupt_names[7] = "No coprocessor exception";
    interrupt_names[8] = "Double fault";
    interrupt_names[9] = "Coprocessor segment overrun";
    interrupt_names[10] = "Bad TSS";
    interrupt_names[11] = "Segment not present";
    interrupt_names[12] = "Stack fault";
    interrupt_names[13] = "General protection fault";
    interrupt_names[14] = "Page fault";
    interrupt_names[15] = "Unknown interrupt exception";
    interrupt_names[16] = "coprocessor fault";
    interrupt_names[17] = "Alignment check exception";
    interrupt_names[18] = "Machine check exception";
    interrupt_names[19] = "Reserved";
    interrupt_names[20] = "Reserved";
    interrupt_names[21] = "Reserved";
    interrupt_names[22] = "Reserved";
    interrupt_names[23] = "Reserved";
    interrupt_names[24] = "Reserved";
    interrupt_names[25] = "Reserved";
    interrupt_names[26] = "Reserved";
    interrupt_names[27] = "Reserved";
    interrupt_names[28] = "Reserved";
    interrupt_names[29] = "Reserved";
    interrupt_names[30] = "Reserved";
    interrupt_names[31] = "Reserved";

    interrupt_names[32] = "Programmable Interrupt Timer Interrupt";
    interrupt_names[33] = "Keyboard Interrupt";
    interrupt_names[34] = "Cascade (used internally by the two PICs. never raised)";
    interrupt_names[35] = "COM2 (if enabled)";
    interrupt_names[36] = "COM1 (if enabled)";
    interrupt_names[37] = "LPT2 (if enabled)";
    interrupt_names[38] = "Floppy Disk";
    interrupt_names[39] = "LPT1 / Unreliable 'spurious' interrupt (usually)";
    interrupt_names[40] = "CMOS real-time clock (if enabled)";
    interrupt_names[41] = "Free for peripherals / legacy SCSI / NIC";
    interrupt_names[42] = "Free for peripherals / SCSI / NIC";
    interrupt_names[43] = "Free for peripherals / SCSI / NIC";
    interrupt_names[44] = "PS2 Mouse";
    interrupt_names[45] = "FPU / Coprocessor / Inter-processor";
    interrupt_names[46] = "Primary ATA Hard Disk";
    interrupt_names[47] = "Secondary ATA Hard Disk";

    interrupt_names[0x80] = "System call";
}

/**
 * Interrupt Service Routine
 * called by isr_common_stub in interrupt.s
 * interrupts are disabled while in this function
 * 
 * This function only handles interrupts 0 to 31 of the IDT
 */
void isr_handler(registers_t regs) {
    putstr("received interrupt ");
    putnbr(regs.int_no);
    putstr(" : ");
    putstr(interrupt_names[regs.int_no]);
    putchar('\n');

    if (regs.err_code) {
        putstr("With error code 0x");
        puthexa(regs.err_code);
        putchar('\n');
    }
}


/**
 * only receives interrupts 32 to 47 (0x20 to 0x2F)
 */
void irq_handler(registers_t regs) {
    /*
    putstr("Received IRQ ");
    putnbr(regs.int_no);
    putstr(" : ");
    putstr(interrupt_names[regs.int_no]);
    putstr("\n");
    */

    if (regs.int_no < 256 && interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    // EOI
    // Send reset signal to master
    outb(REG_PIC1_CTRL, PIC_EOI);
    // sent by slave PIC
    if (regs.int_no >= 0x28) {
        outb(REG_PIC2_CTRL, PIC_EOI);
    }
}