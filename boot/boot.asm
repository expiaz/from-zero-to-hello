; Read some sectors from the boot disk using our disk_read function
; Changed accordingly to stackoverflow.com/questions/34216893/disk-read-error-while-loading-sectors-into-memory
[org 0x7c00]
[bits 16]

KERNEL_OFFSET equ 0x1000

xor ax, ax
mov ds, ax
mov es, ax
mov [BOOT_DRIVE], dl	; boot drive by BIOS

; setup the stack
mov ax, 0x07E0
cli
mov ss, ax
mov sp, 0x1200
mov sp, bp
sti

;mov bp, 0x9000			; Set the stack.
;mov sp, bp

mov si, MSG_REAL_MODE ;do not use EBX for parameter since the BIOS function may use BX to store some
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
	mov si, MSG_LOAD_KERNEL
	call print_string

	mov bx, KERNEL_OFFSET	;load to address kernel offset
	mov dh, 16				;load first 15 sectors (w/o boot sector)
	mov dl, [BOOT_DRIVE]	;from the boot disk
	call disk_load
	
	ret

[bits 32]
; This is where we arrive after switching to and initialising protected mode.
begin_pm:
	mov ebx, MSG_PROT_MODE
	call putstr			; Use our 32 - bit print routine.

	call KERNEL_OFFSET	;jump to the kernel

	jmp $				; Hang.

; Global variables
BOOT_DRIVE		db 0
MSG_REAL_MODE 	db "Started in 16 - bit Real Mode", 13, 10, 0
MSG_PROT_MODE 	db "Successfully landed in 32 - bit Protected Mode", 0
MSG_LOAD_KERNEL	db "Loading kernel into memory", 13, 10, 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55