#include "PIT.h"
#include "isr.h"
#include "../drivers/screen.h"

/*
The PIT has an internal clock which oscillates at approximately 1.1931MHz. This clock signal is fed through a frequency divider, to modulate the final output frequency. It has 3 channels, each with it's own frequency divider.

Channel 0 is the most useful. It's output is connected to IRQ0.
Channel 1 is very un-useful and on modern hardware is no longer implemented. It used to control refresh rates for DRAM.
Channel 2 controls the PC speaker.

OK, so we want to set the PIT up so it interrupts us at regular intervals, at frequency f. I generally set f to be about 100Hz (once every 10 milliseconds), but feel free to set it to whatever you like. To do this, we send the PIT a 'divisor'. This is the number that it should divide it's input frequency (1.9131MHz) by. It's dead easy to work out:

divisor = 1193180 Hz / frequency (in Hz)
Also worthy of note is that the PIT has 4 registers in I/O space
0x40-0x42 are the data ports for channels 0-2 respectively
and 0x43 is the command port.
*/

// number fo ticks
u32 tick = 0;

// IRQ handler for IRQ0
static void timer_callback(registers_t regs) {
   tick++;
   putstr("Tick: ");
   putnbr(tick);
   putstr("\n");
}

void init_timer(u32 frequency) {
   // Firstly, register our timer callback.
   register_interrupt_handler(IRQ0, &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   u32 divisor = 1193180 / frequency;

   // Send the command byte.
   // This byte (0x36) sets the PIT to repeating mode (so that when the divisor counter reaches zero it's automatically refreshed) and tells it we want to set the divisor value.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   u8 l = (u8)(divisor & 0xFF);
   u8 h = (u8)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}