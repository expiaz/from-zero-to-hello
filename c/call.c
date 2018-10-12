
/*

call func
    push ip+1
    jmp func

ret
    pop return
    jmp return

leave
    mov esp, ebp
    pop ebp

enter
    push ebp
    mov ebp, esp

convention:

call func
enter
body
leave
ret

--caller
push ip+1
jmp func
--callee
push ebp
mov ebp, esp
sub esp, 0x4
--body local
mov [ebp-0x4], 0x5
--ret value
mov eax, [ebp-0x4]
mov esp, ebp
pop ebp
pop ebx
jmp ebx
--caller

*/

void caller_function() {
    callee_function(0xbaba);
}

int callee_function(int a) {
    return a;
}