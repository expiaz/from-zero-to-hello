#include "screen.h"
#include "keyboard.h"
#include "common.h"
#include "IDT.h"
#include "PIT.h"

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