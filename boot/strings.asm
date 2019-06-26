; ----------------------
; 16 bits routines
; ----------------------

[bits 16]

putstr_16:
	;pusha			;save registers
	push ax ;bios parameters
	push bx ;bios internal parameters (may be)
	push si ;string parameter

	mov bx, 0x0007	;BL=WhiteOnBlack BH=Display page 0
	mov ah, 0x0e	;tele-type bios interrupt

.loop:
	mov al, [si]	;get the current char
    cmp al, 0		;is it ending
	je .done

	int 0x10		;bios interrupt
	inc si			;next char
	jmp .loop

.done:
	pop si
	pop bx
	pop ax
	;popa			;restore registers
	ret

; print a newline feed and carriage return (\n\r)
;print_ln:
;	push ax
;	mov ah, 0x0e	;bios interrupt
;	
;	mov al, 0x0a	;newline
;	int 0x10
;
;	mov al, 0x0d
;	int 0x10
;
;	pop ax
;	ret

puthex_16:
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
    mov si, HEX_OUT
    call putstr_16

    popa
    ret

HEX_OUT db '0x0000 ', 0

; ----------------------
; 32 bits routines
; ----------------------

[bits 32]

; Define some constants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; prints a null - terminated string pointed to by EDX
putstr_32:
	pusha
	mov edx, VIDEO_MEMORY 	; Set edx to the start of vid mem.
.loop:
	mov al, [esi] 			; Store the char at ESI in AL
	mov ah, WHITE_ON_BLACK 	; Store the attributes in AH
	cmp al, 0 				; if (al == 0) , at end of string , so
	je .done 				; jump to done
	mov [edx], ax 			; Store char and attributes at current character cell.
	add esi, 1				; Increment ESI to the next char in string.
	add edx, 2				; Move to next character cell in vid mem.
	jmp .loop 				; loop around to print the next char.
.done:
	popa
	ret 					; Return from the function