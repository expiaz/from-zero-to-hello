personnal project OS from scratch
| status             | task                             |
| ------------------ | -------------------------------- |
| :heavy_check_mark: | legacy bootloader                |
| :x:                | Multiboot header                 |
| :x:                | retrieve BIOS structure          |
| :x:                | stage1                           |
| :heavy_check_mark: | * setup segments                 |
| :heavy_check_mark: | * disk load stage2               |
| :x:                | stage2                           |
| :heavy_check_mark: | * A20                            |
| :x:                | * memory map                     |
| :heavy_check_mark: | * GDT                            |
| :heavy_check_mark: | * Protected Mode                 |
| :heavy_check_mark: | * Kernel entry ASM               |
| :heavy_check_mark: | * Kernel jump C 32 bits          |
| :heavy_check_mark: | screen driver                    |
| :heavy_check_mark: | IDT                              |
| :x:                | NMI / spurious IRQs              |
| :heavy_check_mark: | remap PIC                        |
| :heavy_check_mark: | ISR 0x0 - 0x20                   |
| :heavy_check_mark: | IRQ 0x20 - 0x2f                  |
| :heavy_check_mark: | keyboard driver                  |
| :heavy_check_mark: | PIT                              |
| :x:                | VMM                              |
| :x:                | * ID mapping                     |
| :x:                | * kernel pages high half         |
| :x:                | * dynamic pages processes        |
| :x:                | scheduler                        |
| :x:                | filesystem                       |
| :x:                | * SATA driver                    |
| :x:                | * FAT/NTFS                       |
| :x:                | usermode                         |
| :x:                | syscalls                         |
| :x:                | common handler                   |
| :x:                | * ABI                            |
| :x:                | * network                        |
| :x:                | * NIC/PCI driver                 |
| :x:                | * network card driver (rtl18139) |
| :x:                | * TCP/IP stack                   |
| :x:                | * Ethernet                       |
| :x:                | * ARP                            |
| :x:                | * ICMP                           |
| :x:                | * IP                             |
| :x:                | * TCP                            |
| :x:                | * UDP? (DNS)                     |
| :x:                | * HTTP?                          |
| :x:                | userland                         |
| :x:                | multicore                        |
| :x:                | shell                            |
| :x:                | API / ELF                        |
| :x:                | GUI                              |