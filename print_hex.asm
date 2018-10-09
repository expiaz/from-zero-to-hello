print_hex:
    pusha
    mov bx, 5       ;go to the end of the string
.loop:
    ;isolate last number
    mov ax, dx
    and ax, 0xf
    ;convert to ascii and store into HEX_OUT
    add ax, 48
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