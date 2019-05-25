[bits 32]
[extern isr_handler]


%macro ISR_NOERRCODE 1
	[global isr%1]
	isr%1:
		cli
		push 0
		push %1
		jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
	[global isr%1]
	isr%1:
		cli
		push %1
		jmp isr_common_stub
%endmacro

isr_common_stub:
	; Save CPU state
	pusha ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
	mov ax, ds ; save the data segment descriptor
	push eax
	mov ax, 0x10 ; load kernel data segment descriptor
	mov ds, ax ; update all segment registers related to data
	mov es, ax
	mov fs, ax
	mov gs, ax

	call isr_handler ; call C handler

	; restore state
	pop eax
	mov ds, ax ; get old data segment 
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8 ; clear the error code and isr number pushed before
	sti
	iretd ; iret double for 32 bit op size => pops CS, EIP, EFLAGS, SS, ESP

ISR_NOERRCODE	0	; Division by zero exception
ISR_NOERRCODE	1	; Debug exception
ISR_NOERRCODE	2	; Non maskable interrupt
ISR_NOERRCODE	3	; Breakpoint exception
ISR_NOERRCODE	4	; into detected overflow
ISR_NOERRCODE	5	; Out of bounds exception
ISR_NOERRCODE	6	; Invalid opcode exception
ISR_NOERRCODE	7	; No coprocessor exception
ISR_ERRCODE		8	; Double fault
ISR_NOERRCODE	9	; Coprocessor segment overrun
ISR_ERRCODE		10	; Bad TSS
ISR_ERRCODE		11	; Segment not present
ISR_ERRCODE		12	; Stack fault
ISR_ERRCODE		13	; General protection fault
ISR_ERRCODE		14	; Page fault
ISR_NOERRCODE	15	; Unknown interrupt exception
ISR_NOERRCODE	16	; coprocessor fault
ISR_NOERRCODE	17	; Alignment check exception
ISR_NOERRCODE	18	; Machine check exception
ISR_NOERRCODE	19	; Reserved
ISR_NOERRCODE	20	; Reserved
ISR_NOERRCODE	21	; Reserved
ISR_NOERRCODE	22	; Reserved
ISR_NOERRCODE	23	; Reserved
ISR_NOERRCODE	24	; Reserved
ISR_NOERRCODE	25	; Reserved
ISR_NOERRCODE	26	; Reserved
ISR_NOERRCODE	27	; Reserved
ISR_NOERRCODE	28	; Reserved
ISR_NOERRCODE	29	; Reserved
ISR_NOERRCODE	30	; Reserved
ISR_NOERRCODE	31	; Reserved
