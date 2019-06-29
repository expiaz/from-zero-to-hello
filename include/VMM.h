#ifndef VMM_H
#define VMM_H

#include "common.h"

#endif // VMM_H

// [https://wiki.osdev.org/Paging]

// cr3 = @ of 1st PDE entry
u32 cr3 = 0xFFFFF000;

void *vaddr_tr(void * vaddr) {

    u32 *pd = (u32 *) 0xFFFFF000;
    if (*pd & 0x1 == 0) return;

    u32 pde = pd[((u32) vaddr >> 22)];
    if (pde & 0x1 == 0) return;

    u32 *pt = (u32 *) (pde & ~0xFFF);
    if (*pt & 0x1 == 0) return;

    u32 pte = pt[(u32) vaddr >> 12 & 0x3FF];
    if (pte & 0x1 == 0) return;

    u8 *page = (u8 *) (pte & ~0xFFF);
    void *addr = (void *) page[(u32) vaddr & 0xFFF];

    /*
        higher half kernel ?
        the page directory of the kernel is on the upper bounds of
        the memory
        as we are in 32bits, it means 4GB

        memory map fo the upper 4GB
        |---------------|---------------|-----------|-----------------------------------------------|
        | start         | end			| size		| type											|
        |---------------|---------------|-----------|---------------------------------------------	|
        | 0xFFC00000	| 0xFFFFEFFF	| 4 MiB     | page table (1024 entries of 1024 entries)     |
        | 0xFFFFF000	| 0xFFFFFFFF	| 4 KiB		| page directory (1024 entries of 4B)			|
        |-------------|---------------|-------------|-----------------------------------------------|
    */

    // mapping is done on the higher addresses
    // PD is at 0xFFFFF000 - 0xFFFFFFFF (0x1000 or 4096 bytes)
    // i.e 1024 entries (1024 = 2^10 => 10 bits in vaddr)
    // of 32 bits entry each (32 bits = 4 bytes => 4 * 1024 = 4096 bytes)

    // PT is at 0xFFC00000 - 0xFFFFF000
    // i.e. 0x3FF000 or 4MiB minus 4096B of the PD above
    // (0xFFFFFFFF - 0xFFC00000 = 0x4000000 or 4MiB)
    // it's just less than 4MiB because at maximum
    // (when the PD & PT are full) we occupy
    // 1024 (PD entries) * 1024 (PT entries by PD entry) * 4 (Bytes per entry)
    // => 0x400000 or 4Mib

    // pages are the rest of the memory
    // 1024 (PDEs) * 1024 (PTEs) * 4096 (pages)
    // => 0x100000000 or 4Gib or 2^32
    // so the full @ space is occupied

    // with vaddr = 0xDEADBEEF (11011110101011011011111011101111)
    // and physical page = 0xABCDE000 (4096B aligned)

    // first 10 bits of vaddr = index of PDE entry
    // in our example "(vaddr >> 22)" would be 1101111010 or 890
    // so this is the n890 PDE and the @ is 
    // "cr3 + 4 * (vaddr >> 22)" = 0xFFFFF000 + 4 * 890 = 0xFFFFFDE8
    // u32 *PDE = (u32 *) 0xFFFFFDE8;
    u32 *PDE = ((u32 *) cr3) + ((u32) vaddr >> 22);

    // bits 21 - 12 of vaddr = index of PTE entry
    // in our example "(vaddr >> 12 & 0x3FF)" would be
    // 11011110101011011011 & 1111111111 = 1011011011 or 731
    // so this is the n731 PTE and the @ is 
    // *0xFFFFFDE8 points to 0xFFF7A000 (0xFFC00000 (base PT @) + 890 (PDE index) * 1024 (nb of PTE per PDE) * 4 (nb of B per PTE), 12 bits of flags cleared for example)
    // "(*PDE & ~0xFFF) + (vaddr >> 12 & 0x3FF)" = (0xFFF7A000 & 11111111111111111111000000000000) + 731 * 4 = 0xFFF7AB6C
    // u32 *PTE = (u32 *) 0xFFF7AB6C;
    u32 *PTE = (u32 *) ((*PDE & ~0xFFF) + ((u32) vaddr >> 12 & 0x3FF));

    // last 12 bits of vaddr = offset in page
    // in our example "(vaddr & 0xFFF)" would be 111011101111 or 0xEEF
    // so the @ of the data is at offset 3823 (0xEEF) in the 4096 (0x1000) page
    // *0xFFF7AB6C points to the PTE of the page i.e. 0xABCDE000 (12 bits of flags cleared for example)
    // "(*PTE & ~0xFFF) + (vaddr & 0xFFF)" = 11111111111111111111000000000000 + 000000000000000000000000111011101111
    // physical address = 0xABCDEEEF
    return (void *) ((*PTE & ~0xFFF) + ((u32) vaddr & 0xFFF));

    return (void *) ((*((u32 *) ((*((u32 *) (cr3 + 4 * ((u32) vaddr >> 22))) & ~0xFFF) + ((u32) vaddr >> 12 & 0x3FF))) & ~0xFFF) + ((u32) vaddr & 0xFFF));

}