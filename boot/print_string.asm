print_string:
	pusha			;save registers
	mov ah, 0x0e	;tele-type bios interrupt

.loop:
	mov al, [bx]	;get the current char
    cmp al, 0		;is it ending
	je .done
	
	int 0x10		;bios interrupt
	inc bx			;next char
	jmp .loop

.done:
	call print_ln
	popa			;restore registers
	ret

; print a newline feed and carriage return (\n\r)
print_ln:
	mov ah, 0x0e	;bios interrupt
	
	mov al, 0x0a	;newline
	int 0x10

	mov al, 0x0d
	int 0x10

	ret