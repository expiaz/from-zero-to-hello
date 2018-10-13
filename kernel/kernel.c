#include "../drivers/screen.h"
#include "common.h"

void dummy() {
    // kernel code isn't first instruction anymore
}

void main() {
    //clear();
    //set_color(RED, BLACK);
    init_screen(0, 0, WHITE, BLACK);
    clear();
    putchar('C');
    /*
    int i = 0;
    for (i = 0; i < 25; i++)
        putstr("01234567890123456789012345678901234567890123456789012345678901234567890123456789");
    putstr("Hi there");
    */
}