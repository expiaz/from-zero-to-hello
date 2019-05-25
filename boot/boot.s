[org 0x7c00]	; start address of our program
				; because BIOS will load the bootloader at this address
				; and then jump to it
[bits 16]

xor ax, ax
mov ds, ax 				; setting up segments
mov es, ax
mov [BOOT_DRIVE], dl	; boot drive given by BIOS

; --------------- setup the stack
; to be able to use call and ret / push pop instructions

; we'll set the stack just after the boot sector in memory
; boot sector is at 0x7c00 and ends at 0x7e00 (0x7c00 + 512)
; so our SS will be 0x07e0 (0x7e00 after address translation)

; if we look at how the lower memory i.e. the first accessible
; MiB, the boot loader is from 0x7c00 to 0x7e00,
; then until 0x9fc00 there is an empty room of 638 KiB
; we'll take only 0x1200 bytes (4608) for the moment for
; our stack and may upgrade later when we'll be in protected mode

; so our stack will be above the bootloader
; at 0x7e00 and ends at 0x9000

mov ax, 0x07e0 ; start of the stack bf address translation (>> 4)
cli
mov ss, ax
mov sp, 0x1200 ; end of the stack
mov sp, bp     ; stack is empty for the moment
sti

mov si, MSG_REAL_MODE ; do not use EBX for parameter since the BIOS function may use BX to store some
call print_string


; -------------- load the kernel
mov si, MSG_LOAD_KERNEL
call print_string

mov bx, KERNEL_ADDR		; address to load kernel
mov dh, 16				; load first 15 sectors (w/o boot sector)
mov dl, [BOOT_DRIVE]	; from the boot disk
call disk_load

; -------------- switching to protected mode
cli						; We must switch of interrupts until we have
                        ; set-up the protected mode interrupt vector
                        ; otherwise interrupts will run riot.
lgdt [gdt_descriptor]   ; Load our global descriptor table , which defines
                        ; the protected mode segments ( e.g. for code and data )

mov eax, cr0			; To make the switch to protected mode , we set
or eax, 0x1			    ; the first bit of CR0 , a control register
mov cr0, eax
jmp CODE_SEG:pm	; Make a far jump ( i.e. to a new segment )
						; to our 32 - bit code. This also forces the CPU
						; to flush its cache of pre - fetched and
						; real - mode decoded instructions
						; which can cause problems.

; ----------- we are now in protected mode
[bits 32]
; Initialise registers and the stack once in PM.
pm:
mov ax, DATA_SEG 	; Now in PM , our old segments are meaningless ,
mov ds, ax			; so we point our segment registers to the
mov ss, ax 			; data selector we defined in our GDT	
mov es, ax
mov fs, ax
mov gs, ax
mov ebp, 0x90000 	; Update our stack position so it is right
mov esp, ebp		; at the top of the free space between the boot
					; loader (0x7e00) and the extended BIOS area (9fc00)
					; so we'll dispose of 0x0:0x90000 - 0x0:0x7e00
					; = 557 568 bytes for ou stack

mov ebx, MSG_PROTECTED_MODE
call putstr			; Use our 32 - bit print routine.

call KERNEL_ADDR	; jump to the kernel
					; actually jumps to kernel_entry.s
					; whose calling the "real" main
					; thus not letting us assume
					; a C function position at 0x1000
					; which the compiler may not
					; have respected

jmp $				; hang forever

%include "gdt.s"
%include "print_string.s"
%include "print_hex.s"
%include "disk_load.s"
%include "putstr.s"

; Global variables
KERNEL_ADDR		equ 0x1000 	; kernel will be loaded at ES:0x1000
							; ES is 0x0, and 0x1000 land in between the
							; BIOS Data Area (0x400 - 0x500) and 
							; the boot loader (0x7c00 - 0x7e00)
							; so we got a maximum of 0x7c00 - 0x1000
							; = 27 648 bytes
							; actually we'll only load the first 15 sectors
							; so 15*512 = 7 680 bytes so it fits
							; but later we may want to rellocate the kernel
							; after the booloader (and the stack) at
							; 0x7e00 + stack size (0x1200 currently)
							; to have more room (638 KiB free)
BOOT_DRIVE			db 0
MSG_REAL_MODE 		db "Started in 16 - bit Real Mode", 13, 10, 0
MSG_PROTECTED_MODE 	db "Successfully landed in 32 - bit Protected Mode", 0
MSG_LOAD_KERNEL		db "Loading kernel into memory", 13, 10, 0

; Bootsector padding (first 510 bytes)
times 510-($-$$) db 0
; magic number for bootable drive
dw 0xaa55