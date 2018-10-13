print_hex:
    pusha
    mov bx, 5       ;go to the last index of string HEX_OUT

.loop:
    ;isolate last number
    mov ax, dx
    and ax, 0xf

    ;convert to ascii by adding '0'
    add ax, 48
    ;if converted number exceed 9, convert to hex
    cmp ax, 57
    jle .store
    ;transform to hex, 57 is '9', 'A' is 65
    ; 65-58 = 7
    add ax, 7

    ;store into HEX_OUT
.store:
    mov [HEX_OUT+bx], al

    ;next number
    shr dx, 4
    ;update index
    dec bx

    ;stop at 'x'
    cmp bx, 1
    jne .loop

    ;print the whole number
    mov bx, HEX_OUT
    call print_string

    popa
    ret

HEX_OUT: db '0x0000 ', 0