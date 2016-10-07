; *********************************************************
;     DankOS BOOTLOADER:  Loads STAGE2.BIN at 0000:0500
; *********************************************************

org 0x7C00						; BIOS loads us here
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

cli								; Disable interrupts and initialise all segments to 0x0000
jmp 0x0000:initialise_cs		; Initialise CS to 0x0000 with a long jump
initialise_cs:
xor ax, ax
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov sp, 0xFFF0					; Stack at segment top (0000:FFF0)
sti								; Restore interrupts

mov si, stage2_name				; Load stage 2 file name pointer in SI, for use by FAT12 function
mov bx, 0x0500					; And tell function to load the stage 2 at es:bx (0000:0500)

call fat12_load_file			; Call load file function

jc $							; If error (the carry flag is set), cleanely halt the system

jmp 0x0000:0x0500				; Otherwise jump to the newly loaded stage 2 :D

%include 'includes/boot_functions/boot_disk_functions.inc'	; Include disk and FAT12 functions
%include 'includes/boot_functions/boot_fat12_functions.inc'

stage2_name					db 'STAGE2  BIN'	; The full FAT12 name of stage 2
times 510-($-$$)			db 0x00				; Fill rest with 0x00
bios_signature				dw 0xAA55			; BIOS signature
