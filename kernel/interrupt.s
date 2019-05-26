[bits 32]
[extern isr_handler]
[extern irq_handler]

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


; -------------- IRQs

; This macro creates a stub for an IRQ - the first parameter is
; the IRQ number, the second is the ISR number it is remapped to.
%macro IRQ 2
  global irq%1
  irq%1:
    cli
    push 0
    push %2
    jmp irq_common_stub
%endmacro

irq_common_stub:
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

IRQ		0,	32
IRQ		1,	33
IRQ		2,	34
IRQ		3,	35
IRQ		4,	36
IRQ		5,	37
IRQ		6,	38
IRQ		7,	39
IRQ		8,	40
IRQ		9,	41
IRQ		10,	42
IRQ		11,	43
IRQ		12,	44
IRQ		13,	45
IRQ		14,	46
IRQ		15,	47