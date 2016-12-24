; ****************************************************************************
;     This file contains routines that are global for both FAT12 and FAT16
; ****************************************************************************


get_current_directory:

; ***********************************************************
;     Returns the present working directory of the kernel
; ***********************************************************

; OUT: ES:DI = Target buffer to dump path in, must be 128+1 bytes long to avoid overflows.

push ax
push si
push ds

mov ax, 0x9000
mov ds, ax

mov si, CurrentDirectoryPath

push 0x27
int 0x80

.done:

pop ds
pop si
pop ax
iret

set_current_directory:

; ********************************************************
;     Sets the present working directory of the kernel
; ********************************************************

; IN: DS:SI = New path

; OUT: DL = 0 if dir not found, 1 if found

push ax
push si
push di
push ds
push es

mov ax, 0x9000
mov es, ax

; Check if the path only points to the root

push si
lodsw
pop si
cmp ax, 0x002F		; Check for / + 0x00
je .just_root

push 0x13		; Get current drive
int 0x80

mov dh, 1		; Look for a dir entry

call fat_path_resolver

test ax, ax
jz .not_found

mov di, .ConvertedPath

call fat_path_converter

mov ax, 0x9000
mov ds, ax
mov si, .ConvertedPath
mov di, CurrentDirectoryPath

push 0x27
int 0x80

.found:
mov dl, 1
jmp .done

.not_found:
xor dl, dl
jmp .done

.just_root:
mov dl, 1
mov word [es:CurrentDirectoryPath], 0x002F

.done:

pop ds
pop es
pop di
pop si
pop ax
iret


.ConvertedPath		times 129 db 0x00



load_file:

iret

file_exists:


iret

fat_load_root:

; **************************************************************
;     Loads the root directory (/) into the directory buffer
; **************************************************************

; IN: DL = drive number

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

mov ebx, 0x0E			; Address of the Reserved sectors constant
push 0x25
int 0x80				; Load word from address

mov word [.StartOfFAT], ax		; Save result

mov ebx, 0x10			; Address of the Number of FATs constant
push 0x24
int 0x80				; Load word from address

mov byte [.NumberOfFATs], al	; Save result

mov ebx, 0x11			; Address of the Root entries constant
push 0x25
int 0x80				; Load word from address

mov word [.RootEntries], ax		; Save result

mov ebx, 0x16			; Address of the Sectors per FAT constant
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
mov ax, word [.StartOfRoot]			; Load from here
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

ret

.SizeOfFAT				dw	0x0000
.CurrentDrive			db	0x00
.StartOfFAT				dw	0x0000
.NumberOfFATs			db	0x00
.StartOfRoot			dw	0x0000
.SizeOfRoot				dw	0x0000
.RootEntries			dw	0x0000
.BytesPerSector			dw	512
