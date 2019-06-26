#include "./keyboard.h"

char keyboard_US[128] = {
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
        char c = keyboard_US[scancode];
        if (c) putchar(c);
    }
}

void init_keyboard() {
    keyboard_US[0] = 0;
    keyboard_US[1] = 27;
    keyboard_US[2] = '1';
    keyboard_US[3] = '2';
    keyboard_US[4] = '3';
    keyboard_US[5] = '4';
    keyboard_US[6] = '5';
    keyboard_US[7] = '6';
    keyboard_US[8] = '7';
    keyboard_US[9] = '8';
    keyboard_US[10] = '9';
    keyboard_US[11] = '0';
    keyboard_US[12] = '-';
    keyboard_US[13] = '=';
    keyboard_US[14] = '\b';
    keyboard_US[15] = '\t';
    keyboard_US[16] = 'q';
    keyboard_US[17] = 'w';
    keyboard_US[18] = 'e';
    keyboard_US[19] = 'r';
    keyboard_US[20] = 't';
    keyboard_US[21] = 'y';
    keyboard_US[22] = 'u';
    keyboard_US[23] = 'i';
    keyboard_US[24] = 'o';
    keyboard_US[25] = 'p';
    keyboard_US[26] = '[';
    keyboard_US[27] = ']';
    keyboard_US[28] = '\n';
    keyboard_US[29] = 0;
    keyboard_US[30] = 'a';
    keyboard_US[31] = 's';
    keyboard_US[32] = 'd';
    keyboard_US[33] = 'f';
    keyboard_US[34] = 'g';
    keyboard_US[35] = 'h';
    keyboard_US[36] = 'j';
    keyboard_US[37] = 'k';
    keyboard_US[38] = 'l';
    keyboard_US[39] = ';';
    keyboard_US[40] = '\'';
    keyboard_US[41] = '`';
    keyboard_US[42] = 0;
    keyboard_US[43] = '\\';
    keyboard_US[44] = 'z';
    keyboard_US[45] = 'x';
    keyboard_US[46] = 'c';
    keyboard_US[47] = 'v';
    keyboard_US[48] = 'b';
    keyboard_US[49] = 'n';
    keyboard_US[50] = 'm';
    keyboard_US[51] = ',';
    keyboard_US[52] = '.';
    keyboard_US[53] = '/';
    keyboard_US[54] = 0;
    keyboard_US[55] = '*';
    keyboard_US[56] = 0;
    keyboard_US[57] = ' ';
    keyboard_US[58] = 0;
    keyboard_US[59] = 0;
    keyboard_US[60] = 0;
    keyboard_US[61] = 0;
    keyboard_US[62] = 0;
    keyboard_US[63] = 0;
    keyboard_US[64] = 0;
    keyboard_US[65] = 0;
    keyboard_US[66] = 0;
    keyboard_US[67] = 0;
    keyboard_US[68] = 0;
    keyboard_US[69] = /* < ... F10 */ 0;
    keyboard_US[70] = /* 69 - Num lock*/ 0;
    keyboard_US[71] = /* Scroll Lock */ 0;
    keyboard_US[72] = /* Home key */ 0;
    keyboard_US[73] = /* Up Arrow */ 0;
    keyboard_US[74] = /* Page Up */ '-';
    keyboard_US[75] = 0;
    keyboard_US[76] = /* Left Arrow */ 0;
    keyboard_US[77] = 0;
    keyboard_US[78] = /* Right Arrow */ '+';
    keyboard_US[79] = 0;
    keyboard_US[80] = /* 79 - End key*/ 0;
    keyboard_US[81] = /* Down Arrow */ 0;
    keyboard_US[82] = /* Page Down */  0;
    keyboard_US[83] = /* Insert Key */ 0;
    keyboard_US[84] = /* Delete Key */ 0;
    keyboard_US[85] = 0;
    keyboard_US[86] = 0;
    keyboard_US[87] = 0;
    keyboard_US[88] = /* F11 Key */ 0;
    keyboard_US[89] = 0; /* F12 Key */

    register_interrupt_handler(IRQ1, &keyboard_handler);
}