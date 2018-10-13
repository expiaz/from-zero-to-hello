#include "common.h"

/**
 * read a byte from a port
 * @param {u16} port the address of the memory mapped I/O port
 * @return {u8} the 8 bits
 */
u8 inb(u16 port) {
    // =a (result) means put AL register in variable result after
    // d (port) means load EDX with variable port before
    u8 result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/**
 * send a byte to a port
 * @param {u16} port
 * @param {u8} data to be sent
 */
void outb(u16 port, u8 data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

/**
 * read a word from a port
 * @param {u16} port the address of the memory mapped I/O port
 * @return {u16} the 16 bits
 */
u16 inw(u16 port) {
    u16 result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

/**
 * send a word to a port
 * @param {u16} port
 * @param {u16} data to be sent
 */
void outw(u16 port, u16 data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

/**
 * copy a chunk of memory to another location
 */
void memcpy(s8 *src, s8 *dest, int bytes) {
    int i;

    i = 0;
    while (i < bytes) {
        dest[i] = src[i];
        i++;
    }
}