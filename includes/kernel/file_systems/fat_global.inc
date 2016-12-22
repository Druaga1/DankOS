; ****************************************************************************
;     This file contains routines that are global for both FAT12 and FAT16
; ****************************************************************************


fat_load_root:

; **************************************************************
;     Loads the root directory (/) into the directory buffer
; **************************************************************

; IN: DL = Drive number, EBX = Partition offset

push eax
push ebx
push ecx
push edx

push ds
push es

mov ax, 0x9000
mov ds, ax
mov es, ax

; Fetch some metadata from the BPB

mov byte [.CurrentDrive], dl

mov dword [.PartitionOffset], eax

							; Get partition offset in bytes
mov ebx, 512
mul ebx
mov dword [.PartitionOffsetInBytes], eax

mov ebx, 0x0E			; Address of the Reserved sectors constant+offset
add ebx, dword [.PartitionOffsetInBytes]
push 0x25
int 0x80				; Load word from address

mov word [.StartOfFAT], ax		; Save result

mov ebx, 0x10			; Address of the Number of FATs constant+offset
add ebx, dword [.PartitionOffsetInBytes]
push 0x24
int 0x80				; Load word from address

mov byte [.NumberOfFATs], al	; Save result

mov ebx, 0x11			; Address of the Root entries constant+offset
add ebx, dword [.PartitionOffsetInBytes]
push 0x25
int 0x80				; Load word from address

mov word [.RootEntries], ax		; Save result

mov ebx, 0x16			; Address of the Sectors per FAT constant+offset
add ebx, dword [.PartitionOffsetInBytes]
push 0x25
int 0x80				; Load word from address

mov word [.SizeOfFAT], ax		; Save result

;  Calculate the start and size of the root directory

; Start = reserved_sectors + (number_of_FATs * sectors_per_FAT)
; Size = (root_entries * 32) / bytes_per_sector

; Number of fats * sector per fat in AX

mov ax, word [.SizeOfFAT]
mov bl, byte [.NumberOfFATs]
xor bh, bh
mul bx

add ax, word [.StartOfFAT]			; Add reserved sectors

mov word [.StartOfRoot], ax				; Save result in memory

; Root entries * 32

mov ax, 32
mul word [.RootEntries]

xor dx, dx								; XOR DX for division
div word [.BytesPerSector]

mov word [.SizeOfRoot], ax			; Save result in memory

; Load root dir into buffer

mov bx, CurrentDirectoryCache
xor eax, eax
mov ax, word [.StartOfRoot]			; Load from here
add eax, dword [.PartitionOffset]
mov cx, word [.SizeOfRoot]			; Load this many sectors
mov dl, byte [.CurrentDrive]			; Retrieve drive
push 0x23
int 0x80

pop es
pop ds

pop edx
pop ecx
pop ebx
pop eax

iret		;	(delete iret and put ret later)

.SizeOfFAT				dw	0x0000
.CurrentDrive			db	0x00
.PartitionOffset		dd	0x00000000
.PartitionOffsetInBytes	dd	0x00000000
.StartOfFAT				dw	0x0000
.NumberOfFATs			db	0x00
.StartOfRoot			dw	0x0000
.SizeOfRoot				dw	0x0000
.RootEntries			dw	0x0000
.BytesPerSector			dw	512
