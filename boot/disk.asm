[bits 16]

; Changed accordingly to stackoverflow.com/questions/34216893/disk-read-error-while-loading-sectors-into-memory

;INT 0x13
;BIOS hard drive read and write routine
;parameters (from wikipedia):
;https://en.wikipedia.org/wiki/INT_13H

; Parameters
; BX = buffer pointer
; DL = drive number
; AH = offset of sector
; AL = number of sectors

;DL = drive number (0x00 = 1st floppy or A:)
;AH = function (0x02 = read sectors from drive)

;For AH = 0x02 expected parameters :
;- AL = number of sectors to read (passed from caller in AL)
;- CH = cylinder
;- CL = index of sector to read from (passed from caller in AH)
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

read_disk:
    mov [REMAINING_SECTORS], al ; number of sectors to read stored in global
    mov ch, 0x00                ; Select cylinder 0
    mov dh, 0x00                ; Select head 0
    mov cl, ah                  ; Start reading from sector number in AH

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
    mov cl, 0x01    ; TODO WHY always sector 1 ? bc we change head/cylinder?
    ; TODO useless ? never inc dh so itll block at 1 bc 1^1 = 0
    xor dh, 1       ; next head on diskette
    jnz .next_group
    inc ch          ; next cylinder
    jmp .next_group

.maybe_retry:
    xor bh, bh
    mov bh, ah      ; isolate error code in bn
                    ; ah = error code, dl = disk drive where error from

    mov si, RETRY_MSG
    call putstr_16

    mov ah, 0x00    ; reset disk function
    int 0x13
    dec di
    jnz .retry
    jmp .disk_error

.done:
    ret

.disk_error:
    mov si, DISK_ERROR_MSG
    call putstr_16

    xor dx, dx
    mov dh, bh      ; bh = saved error code from routine 0x02
    call puthex_16
    
    mov si, LN
    call putstr_16

    mov si, DISK_RESET_MSG
    call putstr_16

    xor dx, dx
    mov dh, ah      ; error code from routine 0x00
    call puthex_16

    jmp $

REMAINING_SECTORS       db 0
DISK_ERROR_MSG          db "Disk read error : ", 0
DISK_RESET_MSG          db "Disk reset error : ", 0
DISK_NUMBER_MSG         db "on disk ", 0
LN                      db 13, 10, 0
RETRY_MSG               db "Resetting disk ... ", 0

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