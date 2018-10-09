; Read some sectors from the boot disk using our disk_read function
[org 0x7c00]

mov bp, 0x9000			; Set the stack.
mov sp, bp
mov bx, MSG_REAL_MODE
call print_string
call switch_pm

%include "gdt.asm"
%include "print_string.asm"
%include "pm.asm"
%include "putstr.asm"

[bits 32]
; This is where we arrive after switching to and initialising protected mode.
BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call putstr			; Use our 32 - bit print routine.
	jmp $				; Hang.

; Global variables
MSG_REAL_MODE db "Started in 16 - bit Real Mode", 0
MSG_PROT_MODE db "Successfully landed in 32 - bit Protected Mode", 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55