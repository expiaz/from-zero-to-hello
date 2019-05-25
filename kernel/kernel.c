#include "../drivers/screen.h"
#include "./common.h"
#include "./idt.h"

void dummy() {
    // kernel code isn't first instruction anymore
}

void main() {
    //clear();
    //set_color(RED, BLACK);
    init_screen(0, 0, WHITE, BLACK);
    clear();
    //putstr("Hello my friend");
    putstr("Kernel boostraped\n");

    set_idt();
    putstr("IDT loaded\n");

    __asm__("int $3");
    //int a = 4 / 0;
}