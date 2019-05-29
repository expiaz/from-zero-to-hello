#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "./common.h"
#include "./idt.h"
#include "../drivers/PIT.h"

void dummy() {
    // kernel code isn't first instruction anymore
}

void main() {
    //clear();
    //set_color(RED, BLACK);
    init_screen(0, 0, WHITE, BLACK);
    clear();
    putstr("Kernel boostraped\n");

    set_idt();
    putstr("IDT loaded\n");

    init_PIT(0);
    init_keyboard();

    // int a = 4 / 0;
}