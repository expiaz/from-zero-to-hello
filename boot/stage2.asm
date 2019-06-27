; second stage will be loaded at 0x00000500 and up to 0x00007BFF
; there is a space of ~30KiB convientionnal free memory

; we will reserve 4 sectors (512 x 4) for the stage 2
; and so, only use 0x00000500 - 0x00000d00
; the rest will be reserved for memory discovery structures

[org 0x0500]

[bits 16]

mov [BOOT_DRIVE], dl	; boot drive given by stage 1

mov si, MSG_STAGE_2
call putstr_16

; -------------- enable A20 gate
a20:
    mov si, MSG_A20
    call putstr_16

    call enable_a20
    cmp ax, 1
    je .enabled

    ; couldn't activate a20 gate
    mov si, MSG_ERR
    call putstr_16
    jmp $

.enabled:
    mov si, MSG_OK
    call putstr_16

discover_drives?:

discover_memory:

; -------------- load the kernel
load_kernel:
    mov si, MSG_LOAD_KERNEL
    call putstr_16

    ; Parameters
    ; BX = buffer pointer
    ; DL = drive number
    ; AH = offset of sector
    ; AL = number of sectors
    mov bx, KERNEL_ADDR		; address to load kernel
    mov al, 16				; load first 15 sectors (w/o boot sector)
    mov ah, 4				; after the stage2 (2 sectors) and stage1 (bootsector)
    mov dl, [BOOT_DRIVE]	; from the boot disk
    call read_disk

; -------------- switching to protected mode
switch_pm:
    cli						; We must switch off interrupts until we have
                            ; set-up the protected mode interrupt vector
                            ; otherwise interrupts will run riot.
    lgdt [gdt_descriptor]   ; Load our global descriptor table , which defines
                            ; the protected mode segments ( e.g. for code and data )

    mov eax, cr0			; To make the switch to protected mode , we set
    or eax, 0x1			    ; the first bit of CR0 , a control register
    mov cr0, eax
    jmp CODE_SEG:pm			; Make a far jump ( i.e. to a new segment )
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
    mov ebp, 0x8DFF 	; Update our stack position with 0x0 stack segment
    mov esp, ebp

    mov esi, MSG_PROTECTED_MODE
    call putstr_32

    call KERNEL_ADDR	; jump to the kernel
                        ; actually jumps to kernel_entry.s
                        ; whose calling the "real" main
                        ; thus not letting us assume
                        ; a C function position at 0x1000
                        ; which the compiler may not
                        ; have respected

    jmp $				; hang forever

; Global variables
KERNEL_ADDR		equ 0x8E00 	; kernel will be loaded at ES:0x8E00
							; ES is 0x0, and 0x8E00 land in between the
BOOT_DRIVE			db 0
MSG_STAGE_2 		db "Bootloader stage 2      ", 0
MSG_LOAD_STAGE_2    db "Loading kernel          ", 0
MSG_A20             db "Enabling A20            ", 0

MSG_PROTECTED_MODE 	db "32 bit Protected Mode", 13, 10, 0

MSG_OK              db "[OK]", 13, 10, 0
MSG_ERR             db "[ERR]", 13, 10, 0
MSG_LN              db 13, 10, 0

%include "a20.s"
%include "memory.s"
%include "gdt.s"

; Stage 2 padding (2 sectors)
times 1024-($-$$) db 0