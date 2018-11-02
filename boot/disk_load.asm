[bits 16]

disk_load:
    mov [SECTORS], dh   ; number of sectors to read stored in global
    mov ch, 0x00    ; Select cylinder 0
    mov dh, 0x00    ; Select head 0
    mov cl, 0x02    ; Start reading from second sector ( i.e. after the boot sector )

.next_group:
    mov di, 5   ; try maximum 5 times

.retry:
    mov ah, 0x02
    mov al, [SECTORS]
    int 0x13
    jc .maybe_retry
    sub [SECTORS], al   ; how much sectors remaining ?
    jz .done
    mov cl, 0x01    ; always sectors 1
    xor dh, 1       ; next head on disk
    jnz .next_group
    inc ch          ; next cylinder
    jmp .next_group

.maybe_retry:
    xor bh, bh
    mov bh, ah      ; ah = error code, dl = disk drive where error from

    mov si, RETRY_MSG
    call print_string

    mov ah, 0x00    ; reset disk drive
    int 0x13
    dec di
    jnz .retry
    jmp .disk_error

.done:
    ret

.disk_error:
    mov si, DISK_ERROR_MSG
    call print_string

    mov dh, bh      ; ah = error code, dl = disk drive where error from
    call print_hex
    
    jmp $
    

SECTORS     db 0
DISK_ERROR_MSG db "Disk read error : ", 0
RETRY_MSG db "retry ", 0

; load DH sectors to ES:BX from drive DL
;_disk_load:
;    push dx     ; Store DX on stack so later we can recall
;                ; how many sectors were request to be read,
;                ; even if it is altered in the meantime
;
;    mov ah, 0x02    ; BIOS read sector function
;    mov al, dh      ; Read DH sectors
;    mov ch, 0x00    ; Select cylinder 0
;    mov dh, 0x00    ; Select head 0
;    mov cl, 0x02    ; Start reading from second sector ( i.e. after the boot sector )
;
;    int 0x13        ; BIOS interrupt
;    jc .disk_error ; Jump if error ( i.e. carry flag set )
;
;    pop dx          ; Restore DX from the stack
;    cmp dh, al      ; if AL ( sectors read ) != DH ( sectors expected )
;    jne .sectors_error ; display error message
;    ret

;.disk_error:
;    mov bx, DISK_ERROR_MSG
;    call print_string
;    mov dh, ah      ; ah = error code, dl = disk drive where error from
;    call print_hex
;    jmp $

;.sectors_error:
;   mov bx, SECTORS_ERROR_MSG
;    call print_string
;    jmp $

; Variables
;DISK_ERROR_MSG db 'Disk read error !', 0
;SECTORS_ERROR_MSG db 'Incorrect number of sectors read', 0