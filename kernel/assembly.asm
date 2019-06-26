[bits 32]

[global set_page_directory]
set_page_directory:
    push ebp            ; setup new frame
    mov ebp, esp

    mov eax, [esp+4]    ; get the argument pushed via the stack in C
    mov cr3, eax        ; set the new page directory