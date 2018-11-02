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

void set_isr() {
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
}

void isr_handler(register_t regs) {
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