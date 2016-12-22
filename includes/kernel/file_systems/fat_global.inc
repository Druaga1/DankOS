; ****************************************************************************
;     This file contains routines that are global for both FAT12 and FAT16
; ****************************************************************************


get_current_directory:

; ***********************************************************
;     Returns the present working directory of the kernel
; ***********************************************************

; OUT: ES:DI = Target buffer to dump path in, must be 128 bytes large.

push ax
push si
push di
push ds

mov ax, 0x9000
mov ds, ax

mov si, CurrentDirectoryPath

.loop:
lodsb
stosb
test al, al
jz .done
jmp .loop

.done:

pop ds
pop di
pop si
pop ax
iret


set_current_directory:

; ***********************************************************
;     Sets a new present working directory for the kernel
; ***********************************************************

; IN: DS:SI = Path of the directory

; OUT: DL = 0 if not found, 1 if found and loaded

push eax
push ebx
push ecx
push si
push di
push es

push edx

mov ax, 0x9000							; ES to kernel space
mov es, ax

mov word [es:.NewPathAddress], si

; Check if the caller only wants to load the root directory

push si
lodsw
pop si
cmp ax, 0x002F		; Check for / + 0x00
je .only_load_root

; Check if the path is absolute or relative

lodsb
cmp al, '/'
je .absolute_path
dec si
.root_loaded:

; Check if this is the last name in the path (the file name), and extract the name

mov di, .ExtractedName

.name_loop:
lodsb
cmp al, '/'
je .load_directory
test al, al
jz .file_name
stosb
jmp .name_loop

.load_directory:

push si
push ds
mov ax, 0x9000
mov ds, ax
mov si, .ExtractedName
mov di, .ConvertedName
push 0x17
int 0x80

mov si, .ConvertedName
call fat_get_starting_cluster_dir
pop ds
pop si

test ax, ax
jz .not_found

mov bx, CurrentDirectoryCache
push 0x13
int 0x80
call fat12_load_chain

mov di, .ExtractedName

jmp .name_loop

.file_name:

push ds
mov ax, 0x9000
mov ds, ax
mov si, .ExtractedName
mov di, .ConvertedName
push 0x17
int 0x80

mov si, .ConvertedName
call fat_get_starting_cluster_dir
pop ds

test ax, ax
jz .not_found

jmp .found

.absolute_path:

call fat_load_root			; Load the root directory
jmp .root_loaded

.not_found:

; Reset back the old directory

push ds
mov ax, 0x9000
mov ds, ax

mov si, CurrentDirectoryPath		; Call function recursively
push 0x83
int 0x80
pop ds

pop edx
xor dl, dl
jmp .done

.found:

; Load directory in the buffer

mov bx, CurrentDirectoryCache
push 0x13
int 0x80
call fat12_load_chain

; Save new pwd path

mov si, word [es:.NewPathAddress]
mov di, CurrentDirectoryPath

.store_dir_loop:
lodsb
stosb
test al, al
jz .store_dir_done
jmp .store_dir_loop
.store_dir_done:

pop edx
mov dl, 1
jmp .done

.only_load_root:

call fat_load_root			; Load the root directory

mov word [es:CurrentDirectoryPath], 0x002F

pop edx
mov dl, 1

.done:

pop es
pop di
pop si
pop ecx
pop ebx
pop eax
iret


.NewPathAddress				dw	0x0000
.ExtractedName		times 13 db 0x00
.ConvertedName		times 12 db 0x00


load_file:

; ******************************************
;     Loads a file from a specified path
; ******************************************

; IN: Path in DS:SI (0x00 terminated), buffer in ES:BX

; OUT: File size in ECX, 0x00 if not found

push eax
push ebx
push edx
push si
push di
push es
push gs

mov ax, 0x9000
mov gs, ax

mov ax, es
mov word [gs:.TargetSegment], ax
mov word [gs:.TargetBuffer], bx

mov ax, 0x9000
mov es, ax

; Check if the path is absolute or relative

lodsb
cmp al, '/'
je .absolute_path
dec si
.root_loaded:

; Check if this is the last name in the path (the file name), and extract the name

mov di, .ExtractedName

.name_loop:
lodsb
cmp al, '/'
je .load_directory
test al, al
jz .file_name
stosb
jmp .name_loop

.load_directory:

push si
push ds
mov ax, 0x9000
mov ds, ax
mov si, .ExtractedName
mov di, .ConvertedName
push 0x17
int 0x80

mov si, .ConvertedName
call fat_get_starting_cluster_dir
pop ds
pop si

test ax, ax
jz .not_found

mov bx, CurrentDirectoryCache
push 0x13
int 0x80
call fat12_load_chain

mov di, .ExtractedName

jmp .name_loop

.file_name:

push ds
mov ax, 0x9000
mov ds, ax
mov si, .ExtractedName
mov di, .ConvertedName
push 0x17
int 0x80

mov si, .ConvertedName
call fat_get_starting_cluster_file
pop ds

test ax, ax
jz .not_found

mov dword [gs:.FileSize], ecx

mov bx, word [gs:.TargetSegment]
mov es, bx

mov bx, word [gs:.TargetBuffer]
push 0x13
int 0x80
call fat12_load_chain

jmp .done

.absolute_path:

call fat_load_root			; Load the root directory
jmp .root_loaded

.not_found:

mov dword [.FileSize], 0x00

.done:

mov ecx, dword [gs:.FileSize]

pop gs
pop es
pop di
pop si
pop edx
pop ebx
pop eax
iret


.ExtractedName		times 13 db 0x00
.ConvertedName		times 12 db 0x00
.TargetBuffer		dw 0x0000
.TargetSegment		dw 0x0000
.FileSize			dd 0x00000000



file_exists:

; *******************************
;     Checks if a file exists
; *******************************

; IN: Path in DS:SI (0x00 terminated)

; OUT: DL = 1 if it exists, 0 if it doesnt

push eax
push ebx
push ecx
push si
push di
push es

push edx

mov ax, 0x9000
mov es, ax

; Check if the path is absolute or relative

lodsb
cmp al, '/'
je .absolute_path
dec si
.root_loaded:

; Check if this is the last name in the path (the file name), and extract the name

mov di, .ExtractedName

.name_loop:
lodsb
cmp al, '/'
je .load_directory
test al, al
jz .file_name
stosb
jmp .name_loop

.load_directory:

push si
push ds
mov ax, 0x9000
mov ds, ax
mov si, .ExtractedName
mov di, .ConvertedName
push 0x17
int 0x80

mov si, .ConvertedName
call fat_get_starting_cluster_dir
pop ds
pop si

test ax, ax
jz .not_found

mov bx, CurrentDirectoryCache
push 0x13
int 0x80
call fat12_load_chain

mov di, .ExtractedName

jmp .name_loop

.file_name:

push ds
mov ax, 0x9000
mov ds, ax
mov si, .ExtractedName
mov di, .ConvertedName
push 0x17
int 0x80

mov si, .ConvertedName
call fat_get_starting_cluster_file
pop ds

test ax, ax
jz .not_found

jmp .found

.absolute_path:

call fat_load_root			; Load the root directory
jmp .root_loaded

.not_found:

pop edx
xor dl, dl
jmp .done

.found:

pop edx
mov dl, 1

.done:

pop es
pop di
pop si
pop ecx
pop ebx
pop eax
iret


.ExtractedName		times 13 db 0x00
.ConvertedName		times 12 db 0x00




fat_get_starting_cluster_dir:

; *************************************************************************************************
;     Returns the starting cluster of the directory chain of the directory in DS:SI in fat name
; *************************************************************************************************

; IN: DS:DI directory fat name
; OUT: AX starting cluster, 0x00 if not found

push bx
push cx
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

.check_if_directory:

mov ah, byte [es:di]
and ah, 0x10
cmp ah, 0x10		; Test for directory flag
jne .skip_entry		; If not a dir, skip

.retrieve_cluster:

add di, 15
mov ax, word [es:di]	; Retrieve starting cluster
mov word [es:.Cluster], ax
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

pop es
pop di
pop si
pop dx
pop cx
pop bx
ret

.EntryCounter	dw	0x0000
.Cluster		dw	0x0000
.FileName		dw	0x0000



fat_get_starting_cluster_file:

; ***************************************************************************************
;     Returns the starting cluster of the file chain of the file in DS:SI in fat name
; ***************************************************************************************

; IN: DS:DI file fat name
; OUT: AX starting cluster, 0x00 if not found, file size in ECX

push bx
push cx
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

.check_if_directory:

mov ah, byte [es:di]
and ah, 0x10
cmp ah, 0x10		; Test for directory flag
je .skip_entry		; If a dir, skip

.retrieve_cluster:

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
pop cx
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

; IN: nothing

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

push 0x13
int 0x80
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
