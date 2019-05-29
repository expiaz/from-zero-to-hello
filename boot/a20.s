[bits 16]

; Before enabling the A20 with any of the methods described below it is better to test whether the A20 address line was already enabled by the BIOS. This can be achieved by comparing, at boot time in real mode, the bootsector identifier (0xAA55) located at address 0000:7DFE with the value 1 MiB higher which is at address FFFF:7E0E. When the two values are different it means that the A20 is already enabled otherwise if the values are identical it must be ruled out that this is not by mere chance. Therefore the bootsector identifier needs to be changed, for instance by rotating it left by 8 bits, and again compared to the 16 bits word at FFFF:7E0E. When they are still the same then the A20 address line is disabled otherwise it is enabled.

check_a20:
    push ds
    push es
    cli

    xor ax, ax
    mov ds, ax                      ; ds = 0x0000
    not ax
    mov es, ax                      ; es = 0xFFFF
                                    ; used to fetch address > 2^20

    mov ax, [ds:0x7dfe]            ; load the bootsector magic
                                    ; to compare it

    cmp [es:0x7e0e], ax             ; 0x0000:0x7DFE == 0xFFFF:0x7e0e
                                    ; (0xffff * 0x10) + 0x7e0e
                                    ; = 0x107dfe > 100000
                                    ; => 1MiB > compared to 0x7dfe
                                    ; if the values are the same
                                    ; i.e. bootsector magic number
                                    ; the memory wraps so the gate is disabled

    jne .else                       ; otherwise, the gate is
                                    ; already enabled

    rol word [ds:0x7dfe], 0x8       ; 0x0000:0x7DFE is the bootsector
                                    ; word address (0x7c00 + 512 - 2)
                                    ; so if we change the value at this
                                    ; address and 1MiB upper the values changes
                                    ; too (at 0xFFFF:0x7e0e)
                                    ; it means that we memory wraps around 1Mib
                                    ; so the A20 line is disabled

    mov ax, [ds:0x7dfe]
    cmp [es:0x7e0e], ax
    rol word [ds:0x7dfe], 0x8       ; restore the magic number
    jne .else

    mov ax, 0                       ; two values are identical
                                    ; even after modifying only one
                                    ; so memory wraps at the same
                                    ; location
    jmp .return

.else:
    mov ax, 1
    
.return:
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