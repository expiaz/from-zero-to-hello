print_string:
	pusha			;save registers
	mov ah, 0x0e	;tele-type bios interrupt
.loop:
	mov al, [bx]	;get the current char
    cmp al, 0		;is it ending
	je .ending
	int 0x10		;bios interrupt
	inc bx			;next char
	jmp .loop
.ending:
	popa			;restore registers
	ret