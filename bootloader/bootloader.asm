; *******************************************************************************
;     DankOS BOOTLOADER:  Loads STAGE2 from the reserved sectors at 0000:0500
; *******************************************************************************

org 0x7C00						; BIOS loads us here (0000:7C00)
bits 16							; 16-bit real mode code

jmp short code_start			; Jump to the start of the code
nop								; Pad with NOP instruction
times 3-($-$$) db 0x00			; Make sure this is the start of the BPB

; The BPB starts here

bpbOEM						db 'DANK OS '
bpbBytesPerSector			dw 512
bpbSectorsPerCluster		db 1
bpbReservedSectors			dw 5
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
xor ax, ax
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov sp, 0xFFF0					; Stack at segment top (0000:FFF0)
sti								; Restore interrupts

mov ah, 0x02							; Read sector function
mov al, 4								; Read 4 sectors
mov ch, 0								; Track 0
mov cl, 2								; Sector 2
mov dh, 0								; Head 0
mov bx, 0x0500							; Load at 0000:0500

clc										; Clear carry for int 0x13 because some BIOSes may not clear it on success

int 0x13

jc .err

jmp 0x0000:0x0500				; Jump to the newly loaded stage 2

.err:
mov al, '!'
mov ah, 0x0E
int 0x10
jmp $

times 510-($-$$)			db 0x00				; Fill rest with 0x00
bios_signature				dw 0xAA55			; BIOS signature
