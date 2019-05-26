#include "keyboard.h"

char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b',   /* Backspace */
    '\t',           /* Tab */
    'q', 'w', 'e', 'r', /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',       /* Enter key */
    0,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   /* 39 */
    '\'', '`',   0,     /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',         /* 49 */
    'm', ',', '.', '/',   0,                    /* Right shift */
    '*',
    0,  /* Alt */
    ' ',    /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

void keyboard_handler(registers_t regs) {
    u8 scancode = inb(0x60);
    //putchar(scan_code);
    if (scancode & 0x80); // key release
    else {
        // key down
        char c = kbdus[scancode];
        if (c) putchar(c);
    }
}

void init_keyboard() {
    kbdus[0] = 0;
    kbdus[1] = 27;
    kbdus[2] = '1';
    kbdus[3] = '2';
    kbdus[4] = '3';
    kbdus[5] = '4';
    kbdus[6] = '5';
    kbdus[7] = '6';
    kbdus[8] = '7';
    kbdus[9] = '8';
    kbdus[10] = '9';
    kbdus[11] = '0';
    kbdus[12] = '-';
    kbdus[13] = '=';
    kbdus[14] = '\b';
    kbdus[15] = '\t';
    kbdus[16] = 'q';
    kbdus[17] = 'w';
    kbdus[18] = 'e';
    kbdus[19] = 'r';
    kbdus[20] = 't';
    kbdus[21] = 'y';
    kbdus[22] = 'u';
    kbdus[23] = 'i';
    kbdus[24] = 'o';
    kbdus[25] = 'p';
    kbdus[26] = '[';
    kbdus[27] = ']';
    kbdus[28] = '\n';
    kbdus[29] = 0;
    kbdus[30] = 'a';
    kbdus[31] = 's';
    kbdus[32] = 'd';
    kbdus[33] = 'f';
    kbdus[34] = 'g';
    kbdus[35] = 'h';
    kbdus[36] = 'j';
    kbdus[37] = 'k';
    kbdus[38] = 'l';
    kbdus[39] = ';';
    kbdus[40] = '\'';
    kbdus[41] = '`';
    kbdus[42] = 0;
    kbdus[43] = '\\';
    kbdus[44] = 'z';
    kbdus[45] = 'x';
    kbdus[46] = 'c';
    kbdus[47] = 'v';
    kbdus[48] = 'b';
    kbdus[49] = 'n';
    kbdus[50] = 'm';
    kbdus[51] = ',';
    kbdus[52] = '.';
    kbdus[53] = '/';
    kbdus[54] = 0;
    kbdus[55] = '*';
    kbdus[56] = 0;
    kbdus[57] = ' ';
    kbdus[58] = 0;
    kbdus[59] = 0;
    kbdus[60] = 0;
    kbdus[61] = 0;
    kbdus[62] = 0;
    kbdus[63] = 0;
    kbdus[64] = 0;
    kbdus[65] = 0;
    kbdus[66] = 0;
    kbdus[67] = 0;
    kbdus[68] = 0;
    kbdus[69] = /* < ... F10 */ 0;
    kbdus[70] = /* 69 - Num lock*/ 0;
    kbdus[71] = /* Scroll Lock */ 0;
    kbdus[72] = /* Home key */ 0;
    kbdus[73] = /* Up Arrow */ 0;
    kbdus[74] = /* Page Up */ '-';
    kbdus[75] = 0;
    kbdus[76] = /* Left Arrow */ 0;
    kbdus[77] = 0;
    kbdus[78] = /* Right Arrow */ '+';
    kbdus[79] = 0;
    kbdus[80] = /* 79 - End key*/ 0;
    kbdus[81] = /* Down Arrow */ 0;
    kbdus[82] = /* Page Down */  0;
    kbdus[83] = /* Insert Key */ 0;
    kbdus[84] = /* Delete Key */ 0;
    kbdus[85] = 0;
    kbdus[86] = 0;
    kbdus[87] = 0;
    kbdus[88] = /* F11 Key */ 0;
    kbdus[89] = 0; /* F12 Key */

    register_interrupt_handler(IRQ1, &keyboard_handler);
}