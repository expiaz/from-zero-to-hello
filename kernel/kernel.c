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
    //putstr("Hello my friend");
    putstr("Boot from hard drive (y/n) ? ");
}