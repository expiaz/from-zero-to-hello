#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "./common.h"
#include "./IDT.h"
#include "../drivers/PIT.h"

void dummy() {
    // kernel code isn't first instruction anymore
}

void main() {
    screen_init(0, 0, WHITE, BLACK);
    clear();
    putstr("Kernel boostraped\n");

    IDT_init();
    putstr("IDT loaded\n");

    //PIT_init(100);
    init_keyboard();
    
    // VMM_init
    // shell_init
}