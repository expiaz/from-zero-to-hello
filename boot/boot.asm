; Read some sectors from the boot disk using our disk_read function
[org 0x7c00]
KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl	; boot drive by BIOS

mov bp, 0x9000			; Set the stack.
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call load_kernel

call switch_pm

jmp $

%include "gdt.asm"
%include "print_string.asm"
%include "print_hex.asm"
%include "disk_load.asm"
%include "pm.asm"
%include "putstr.asm"

[bits 16]

load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print_string

	mov bx, KERNEL_OFFSET	;load to address kernel offset
	mov dh, 4				;load first 15 sectors (w/o boot sector)
	mov dl, [BOOT_DRIVE]	;from the boot disk
	call disk_load
	
	ret

[bits 32]
; This is where we arrive after switching to and initialising protected mode.
BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call putstr			; Use our 32 - bit print routine.

	call KERNEL_OFFSET	;jump to the kernel

	jmp $				; Hang.

; Global variables
BOOT_DRIVE		db 0
MSG_REAL_MODE 	db "Started in 16 - bit Real Mode", 0
MSG_PROT_MODE 	db "Successfully landed in 32 - bit Protected Mode", 0
MSG_LOAD_KERNEL	db "Loading kernel into memory", 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55