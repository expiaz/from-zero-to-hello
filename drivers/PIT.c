#include "./PIT.h"

/*
The PIT has an internal clock which oscillates at approximately 1.1931MHz. This clock signal is fed through a frequency divider, to modulate the final output frequency. It has 3 channels, each with it's own frequency divider.

I/O port     Usage
0x40         Channel 0 data port (read/write)
0x41         Channel 1 data port (read/write)
0x42         Channel 2 data port (read/write)
0x43         Mode/Command register (write only, a read is ignored)

0x43 register contains
Bits         Usage
 6 and 7      Select channel :
                 0 0 = Channel 0
                 0 1 = Channel 1
                 1 0 = Channel 2
                 1 1 = Read-back command (8254 only)
 4 and 5      Access mode :
                 0 0 = Latch count value command
                 0 1 = Access mode: lobyte only
                 1 0 = Access mode: hibyte only
                 1 1 = Access mode: lobyte/hibyte
 1 to 3       Operating mode :
                 0 0 0 = Mode 0 (interrupt on terminal count)
                 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                 0 1 0 = Mode 2 (rate generator)
                 0 1 1 = Mode 3 (square wave generator)
                 1 0 0 = Mode 4 (software triggered strobe)
                 1 0 1 = Mode 5 (hardware triggered strobe)
                 1 1 0 = Mode 2 (rate generator, same as 010b)
                 1 1 1 = Mode 3 (square wave generator, same as 011b)
 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD

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
void PIT_handler(registers_t regs) {
   tick++;
   putstr("Tick: ");
   putnbr(tick);
   putstr("\n");
}

void PIT_init(u32 frequency) {
   // Firstly, register our timer callback.
   register_interrupt_handler(IRQ0, &PIT_handler);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   u32 divisor = 1193182 / frequency;

   // Send the command byte.
   // This byte (0x36) sets the PIT to repeating mode (so that when the divisor counter reaches zero it's automatically refreshed) and tells it we want to set the divisor value.
   outb(0x43, 0x36); // 00(channel 0) 11(access mode lo/hi byte) 011 (mode 3 square wave generator) 0(16bit binary mode)

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   u8 l = (u8)(divisor & 0xFF);
   u8 h = (u8)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}