;Ensure that we jump into kernel's main function
[bits 32]
[extern main]

call main
jmp $