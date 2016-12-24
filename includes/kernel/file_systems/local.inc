; *************************************************************************************
;     This file contains FAT routines that are only callable from within the kernel
; *************************************************************************************


fat_get_cluster:

; ******************************************************
;     Returns the starting cluster of the file chain
; ******************************************************

; IN: DS:DI file fat name
; OUT: AX starting cluster, 0x00 if not found, file size in ECX

push bx
push dx
push si
push di
push es

mov ax, 0x9000
mov es, ax

mov di, CurrentDirectoryCache
mov word [es:.EntryCounter], 0x0000
mov word [es:.FileName], si

.next_entry:

inc word [es:.EntryCounter]

mov ah, byte [es:di]	; Byte from the directory table, first of entry
test ah, ah		; End of table?
jz .not_found

mov cx, 11

.check_name_loop:

lodsb			; Byte from the file name
mov ah, byte [es:di]	; Byte from table
inc di
cmp al, ah
jne .skip_entry
loop .check_name_loop

; FILE FOUND!!!

add di, 15
mov ax, word [es:di]	; Retrieve starting cluster
mov word [es:.Cluster], ax

; Get file size

add di, 2
mov eax, dword [es:di]
mov dword [es:.FileSize], eax

jmp .done

.skip_entry:

mov ax, 32
mov di, CurrentDirectoryCache
mul word [es:.EntryCounter]
add di, ax
mov si, word [es:.FileName]
jmp .next_entry

.not_found:

mov word [es:.Cluster], 0x0000

.done:

mov ax, word [es:.Cluster]
mov ecx, dword [es:.FileSize]

pop es
pop di
pop si
pop dx
pop bx
ret


.EntryCounter	dw	0x0000
.Cluster		dw	0x0000
.FileName		dw	0x0000
.FileSize		dd	0x00000000



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
