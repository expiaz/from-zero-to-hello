; memory map of lower memory (< 0x100000 or 1MB)
; |-------------|---------------|-----------|---------------------------------------------	|
; | start		| end			| size		| type											|
; |-------------|---------------|-----------|---------------------------------------------	|
; | 0x00000500	| 0x00007BFF	| ~30 KiB	| Conventional memory (guaranteed free for use)	|
; | 0x00007C00	| 0x00007DFF	| 512 bytes | bootsector (reserved)							|
; | 0x00007E00	| 0x0007FFFF	| 480.5 KiB | Conventional memory (guaranteed free for use)	|
; |-------------|---------------|-----------|---------------------------------------------	|

; 0x500 to 0x7FFFF is USABLE
; our usage of lower memory
; |-------------|---------------|-----------|---------------------------------------------	|
; | start		| end			| size		| type											|
; |-------------|---------------|-----------|---------------------------------------------	|
; | 0x00000500	| 0x00000CFF	| 2 KiB		| stage 2										|
; | 0x00000D00	| 0x00001100	| 1 KiB		| bootinfo structure							|
; | 0x00007C00	| 0x00007E00	| 512 bytes | stage 1										|
; | 0x00007E00	| 0x00008DFF	| 4 KiB 	| stack											|
; | 0x00008E00	| 0x0000ADFF	| 8 KiB 	| kernel										|
; | 0x0000AE00	| 0x0007FFFF	| ~480 Kib 	| free	(paging ?)								|
; |-------------|---------------|-----------|---------------------------------------------	|

[org 0x7C00]	; start address of our program
				; because BIOS will load the bootloader at this address
				; and then jump to it
[bits 16]

xor ax, ax
mov ds, ax 				; setting up segments
mov es, ax
mov [BOOT_DRIVE], dl	; boot drive given by BIOS

; --------------- setup the stack
; to be able to use call and ret / push pop instructions

; we'll set the stack just after the boot sector in memory
; boot sector is at 0x7c00 and ends at 0x7e00 (0x7c00 + 512)
; so our SS will be 0x07e0 (0x7e00 after address translation)

mov ax, 0x07E0 ; start of the stack bf address translation (>> 4)
cli
mov ss, ax
mov sp, 0x1000 ; 4KB stack
mov sp, bp     ; stack is empty for the moment
sti

mov si, MSG_STAGE_1
call putstr_16

; -------------- load the second stage
load_stage_2:
	mov si, MSG_LOAD_STAGE_2
	call putstr_16

	; Parameters
	; BX = buffer pointer
	; DL = drive number
	; AH = offset of sector
	; AL = number of sectors
	mov bx, STAGE_2_ADDR
	mov al, 2				; load first 2 sectors (w/o boot sector)
	mov ah, 2				; from second sector
	mov dl, [BOOT_DRIVE]	; from the boot disk
	call read_disk

	mov si, MSG_OK
	call putstr_16

	mov dl, [BOOT_DRIVE]    ; gives stage 2 bios information
	call STAGE_2_ADDR	    ; jump to the stage 2

	jmp $				    ; hang forever

; Global variables
STAGE_2_ADDR        equ 0x0500

BOOT_DRIVE			db 0
MSG_STAGE_1 		db "Bootloader stage 1  ", 0
MSG_LOAD_STAGE_2    db "Loading stage 2     ", 0
MSG_OK              db "[OK]", 13, 10, 0

%include "disk.asm"
%include "strings.asm"

; Bootsector padding (first 510 bytes)
times 510-($-$$) db 0
; magic number for bootable drive
dw 0xaa55

; append the second stage after the boot sector
%include "stage2.asm"