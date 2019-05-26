#include "isr.h"

char *exception_messages[32]; /* = {
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

    
char *IRQ_names[15];

isr_t interrupt_handlers[256];

void register_interrupt_handler(u8 number, isr_t handler) {
  interrupt_handlers[number] = handler;
}

void set_isr() {
    memset((char *) &interrupt_handlers, 0, 256 * sizeof(isr_t));

    exception_messages[0] = "Division by zero exception";
    exception_messages[1] = "Debug exception";
    exception_messages[2] = "Non maskable interrupt";
    exception_messages[3] = "Breakpoint exception";
    exception_messages[4] = "into detected overflow";
    exception_messages[5] = "Out of bounds exception";
    exception_messages[6] = "Invalid opcode exception";
    exception_messages[7] = "No coprocessor exception";
    exception_messages[8] = "Double fault";
    exception_messages[9] = "Coprocessor segment overrun";
    exception_messages[10] = "Bad TSS";
    exception_messages[11] = "Segment not present";
    exception_messages[12] = "Stack fault";
    exception_messages[13] = "General protection fault";
    exception_messages[14] = "Page fault";
    exception_messages[15] = "Unknown interrupt exception";
    exception_messages[16] = "coprocessor fault";
    exception_messages[17] = "Alignment check exception";
    exception_messages[18] = "Machine check exception";
    exception_messages[19] = "Reserved";
    exception_messages[20] = "Reserved";
    exception_messages[21] = "Reserved";
    exception_messages[22] = "Reserved";
    exception_messages[23] = "Reserved";
    exception_messages[24] = "Reserved";
    exception_messages[25] = "Reserved";
    exception_messages[26] = "Reserved";
    exception_messages[27] = "Reserved";
    exception_messages[28] = "Reserved";
    exception_messages[29] = "Reserved";
    exception_messages[30] = "Reserved";
    exception_messages[31] = "Reserved";

    IRQ_names[0] = "Programmable Interrupt Timer Interrupt";
    IRQ_names[1] = "KeyboardInterrupt";
    IRQ_names[2] = "Cascade (used internally by the two PICs. never raised)";
    IRQ_names[3] = "COM2 (if enabled)";
    IRQ_names[4] = "COM1 (if enabled)";
    IRQ_names[5] = "LPT2 (if enabled)";
    IRQ_names[6] = "Floppy Disk";
    IRQ_names[7] = "LPT1 / Unreliable 'spurious' interrupt (usually)";
    IRQ_names[8] = "CMOS real-time clock (if enabled)";
    IRQ_names[9] = "Free for peripherals / legacy SCSI / NIC";
    IRQ_names[10] = "Free for peripherals / SCSI / NIC";
    IRQ_names[11] = "Free for peripherals / SCSI / NIC";
    IRQ_names[12] = "PS2 Mouse";
    IRQ_names[13] = "FPU / Coprocessor / Inter-processor";
    IRQ_names[14] = "Primary ATA Hard Disk";
    IRQ_names[15] = "Secondary ATA Hard Disk";
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
    putstr(exception_messages[regs.int_no]);
    putchar('\n');

    if (regs.err_code) {
        putstr("With error code 0x");
        puthexa(regs.err_code);
        putchar('\n');
    }
}


/**
 * 
 */
void irq_handler(registers_t regs) {
    // sent by slave PIC
    if (regs.int_no > 0x28) {
        outb(PIC_slave_control_port, PIC_EOI);
    }
    // Send reset signal to master
    outb(PIC_master_control_port, PIC_EOI);

    putstr("Received IRQ ");
    putnbr(regs.int_no);
    putstr(" : ");
    putstr(IRQ_names[regs.int_no]);

    if (regs.int_no < 256 && interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}