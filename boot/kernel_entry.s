; Ensure that we jump into kernel's main function
; we dot it via ASM and not C because in ASM
; we control at which address the instructions are emitted
; In C we can't know if the compiler will output something
; before our first line of code
[bits 32]
[extern main]

call main
jmp $