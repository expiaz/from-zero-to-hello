[bits 16]

print_string:
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