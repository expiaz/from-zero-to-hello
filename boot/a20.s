
; Before enabling the A20 with any of the methods described below it is better to test whether the A20 address line was already enabled by the BIOS. This can be achieved by comparing, at boot time in real mode, the bootsector identifier (0xAA55) located at address 0000:7DFE with the value 1 MiB higher which is at address FFFF:7E0E. When the two values are different it means that the A20 is already enabled otherwise if the values are identical it must be ruled out that this is not by mere chance. Therefore the bootsector identifier needs to be changed, for instance by rotating it left by 8 bits, and again compared to the 16 bits word at FFFF:7E0E. When they are still the same then the A20 address line is disabled otherwise it is enabled.

check_a20:
    push ds
    push es
    cli

    xor ax, ax
    mov ds, ax
    not ax
    mov es, ax

    cmp word [es:0x7e0e], 0xaa55
    jne .disabled

    rol word [ds:0x7dfe], 0x8
    cmp word [es:0x7e0e], 0x55aa
    jne .disabled

    mov ax, 1
    jmp .return

.disabled:
    mov ax, 0

.return:
    rol word [ds:0x7dfe], 0x8       ; restore the magic number
    pop es
    pop ds
    sti
    ret

; interrupt 0x15, ah = 0x24 (a20 gate function) al = 0x01 (enable a20 gate)
enable_a20:
    call check_a20
    cmp ax, 1
    je .return

    mov ax, 0x2401  ; activate a20 gate via BIOS
    int 0x15

    jc .error       ; CF set, error occured
    cmp ah, 0
    je .error       ; ah == 0 couldn't activate the gate

    mov ax, 1
    jmp .return

.error:
    ; possible errors stored in AH
    ; 0x01 keyboard controller is in secure mode
	; 0x86 function not supported
    mov ax, 0
    jmp .return

.return:
    ret