; *************************************************************************************************
;     DankOS BOOTLOADER:  Loads the kernel from the reserved sectors to memory (0000:0500)
; *************************************************************************************************

org 0x7C00						; BIOS loads us here (0000:7C00)
bits 16							; 16-bit real mode code

jmp short code_start			; Jump to the start of the code
nop								; Pad with NOP instruction
times 3-($-$$) db 0x00			; Make sure this is the start of the BPB

; The BPB starts here

bpbOEM						db 'DANK OS '
bpbBytesPerSector			dw 512
bpbSectorsPerCluster		db 1
bpbReservedSectors			dw 65
bpbNumberOfFATs				db 2
bpbRootEntries				dw 224
bpbTotalSectors				dw 2880
bpbMedia					db 0xF8
bpbSectorsPerFAT			dw 9
bpbSectorsPerTrack			dw 18
bpbHeadsPerCylinder			dw 2
bpbHiddenSectors			dd 0
bpbTotalSectorsBig			dd 0
bsDriveNumber				db 0x00
bsUnused					db 0x00
bsExtBootSignature			db 0x29
bsSerialNumber				dd 0x12345678
bsVolumeLabel				db 'DANK OS    '
bsFileSystem				db 'FAT12   '

; End of BPB, start of main bootloader code

code_start:

jmp 0x0000:initialise_cs		; Initialise CS to 0x0000 with a long jump
initialise_cs:
xor ax, ax
mov ds, ax

; Move bootloader to 8000:7C00

mov ax, 0x8000
mov es, ax
mov si, 0x7C00
mov di, si
mov cx, 512
rep movsb
jmp 0x8000:new_location

new_location:

cli
mov ds, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov sp, 0xFFF0
sti

mov si, LoadingMsg				; Print loading message using simple print (BIOS)
call simple_print

call enable_a20					; Enable the A20 address line to access high memory
jc err							; If it fails, print an error and halt

mov si, KernelMsg				; Show loading kernel message
call simple_print

push es
xor ax, ax
mov es, ax
mov ax, 1						; Start from LBA sector 1
mov bx, 0x0500					; Load to offset 0x0500
mov cx, 64						; Load 64 sectors (32 KB)
call read_sectors
pop es

jc err							; Catch any error

jmp 0x0000:0x0500				; Jump to the newly loaded kernel

err:
mov si, ErrMsg
call simple_print

halt:
hlt
jmp halt

;Data

LoadingMsg		db 0x0D, 0x0A, 'Loading DankOS...', 0x0D, 0x0A
				db 0x0D, 0x0A, 'Enabling A20 line...', 0x00
KernelMsg		db '  DONE', 0x0D, 0x0A, 'Loading kernel...', 0x00
ErrMsg			db 0x0D, 0x0A, 0x0A, 'Error, system halted.', 0x00

;Includes

%include 'bootloader/functions/simple_print.inc'
%include 'bootloader/functions/disk.inc'
%include 'bootloader/functions/a20_enabler.inc'

times 510-($-$$)			db 0x00				; Fill rest with 0x00
bios_signature				dw 0xAA55			; BIOS signature
