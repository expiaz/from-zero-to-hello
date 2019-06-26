[bits 16]

push es
xor es, es
pop es

MEMORY_LENGTH       dd 0        ; size of the full high memory available
MEMORY_AVAILABLE    dw 0x0D00