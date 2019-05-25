[bits 16]

; Changed accordingly to stackoverflow.com/questions/34216893/disk-read-error-while-loading-sectors-into-memory

;INT 0x13
;BIOS hard drive read and write routine
;parameters (from wikipedia):
;https://en.wikipedia.org/wiki/INT_13H

;DL = drive number (0x00 = 1st floppy or A:)
;AH = function (0x02 = read sectors from drive)

;For AH = 0x02 expected parameters :
;- AL = Sectors to read (passed from caller in DH)
;- CH = cylinder
;- CL = sector
;- DH = head
;- DL = drive (passed from caller in DL)
;- ES:BX = buffer address pointer (/!\ BX + AL * 512 <= 0x10000)
;return
;- CF set on error, clear otherwise
;- AH = return code
;- AL = actual sectors read count

;For AH = 0x00 expected parameters
;- DL = drive
;return
;- CF set on error
;- AH return code

;Addressing of Buffer should guarantee that the complete
;buffer is inside the given segment, i.e. ( BX + sectors * 512 ) <= 0x10000 
;(16 bit segment). Otherwise the interrupt may fail
;with some BIOS or hardware versions.

;we use this function to load 16 sectors from the 2nd into memory
;at address 0x1000.
;0x1000 + 16 * 512 = 0x3000

disk_load:
    mov [REMAINING_SECTORS], dh ; number of sectors to read stored in global
    mov ch, 0x00                ; Select cylinder 0
    mov dh, 0x00                ; Select head 0
    mov cl, 0x02                ; Start reading from second sector ( i.e. after the boot sector )

.next_group:
    mov di, 5   ; counter of tries
                ; try maximum 5 times

.retry:
    mov ah, 0x02                ; read disk function
    mov al, [REMAINING_SECTORS] ; number of sectors to read
    int 0x13
    jc .maybe_retry             ; if carry set then error so retry
    sub [REMAINING_SECTORS], al ; otherwise sub the read sectors from the total
    jz .done
    mov cl, 0x01    ; always sector 1
    xor dh, 1       ; next head on diskette
    jnz .next_group
    inc ch          ; next cylinder
    jmp .next_group

.maybe_retry:
    xor bh, bh
    mov bh, ah      ; ah = error code, dl = disk drive where error from

    mov si, RETRY_MSG
    call print_string

    mov ah, 0x00    ; reset disk function
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

REMAINING_SECTORS       db 0
DISK_ERROR_MSG          db "Disk read error : ", 0
RETRY_MSG               db "retry ", 0

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