; *********************************************************
;     DankOS BOOTLOADER:  Loads KERNEL.SYS at 9000:0100
; *********************************************************

org 0x7C00						; BIOS loads us here (0000:7C00)
bits 16							; 16-bit real mode code

jmp short code_start			; Jump to the start of the code
nop								; Pad with NOP instruction
times 3-($-$$) db 0x00			; Make sure this is the start of the BPB

; The BPB starts here

bpbOEM						db 'DANK OS '
bpbBytesPerSector			dw 512
bpbSectorsPerCluster		db 1
bpbReservedSectors			dw 1
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

cli								; Disable interrupts and initialise DS and SS to 0x0000
jmp 0x0000:initialise_cs		; Initialise CS to 0x0000 with a long jump
initialise_cs:
xor bx, bx
mov ds, bx
mov ss, bx
mov sp, 0xFFF0					; Stack at segment top (0000:FFF0)
sti								; Restore interrupts

mov ax, 0x9000					; Set destination segment (ES) to 0x9000
mov es, ax
mov si, kernel_name				; Load stage 2 file name pointer in SI, for use by FAT12 function
mov bx, 0x0100					; And tell function to load the kernel at es:bx (9000:0100)
								; DL is already loaded with the correct drive

call fat12_load_file			; Call load file function

jc loading_error				; If error (the carry flag is set), cleanely halt the system

jmp 0x9000:0x0100				; Otherwise jump to the newly loaded kernel :D


loading_error:

mov ah, 0x0E					; Print a '!' using the BIOS, then halt
mov al, '!'
int 0x10
.halt:
hlt
jmp .halt


%include 'bootloader/bootloader_functions/boot_disk_functions.inc'	; Include disk and FAT12 functions
%include 'bootloader/bootloader_functions/boot_fat12_functions.inc'

kernel_name					db 'KERNEL  SYS'	; The full FAT12 name of the kernel
times 510-($-$$)			db 0x00				; Fill rest with 0x00
bios_signature				dw 0xAA55			; BIOS signature
