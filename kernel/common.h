
#ifndef COMMON_H
#define COMMON_H

typedef unsigned    char    u8;
typedef             char    s8;
typedef unsigned    short   u16;
typedef             short   s16;
typedef unsigned    int     u32;
typedef             int     s32;
typedef unsigned    long    u64;
typedef             long    s64;

u8 inb(u16 port);
void outb(u16 port, u8 data);
u16 inw(u16 port);
void outw(u16 port, u16 data);

void memcpy(s8 *src, s8 *dest, s32 bytes);
void memset(s8 *src, u8 value, int size);

#endif