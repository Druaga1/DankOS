; **********************************************************************************************
;     This file contains FAT routines that are callable from outside programs using int 0x80
; **********************************************************************************************



load_file:

; ***********************************************
;     Loads a file from the current directory
; ***********************************************

; IN:
; es:bx		-->		Target segment:offset
; ds:si		-->		Must point to a filename (readable format)

; OUT:
; ecx		-->		File size (in bytes)
; dl		-->		0xFF if failure, cleared on success

push ax
push bx
push si
push di
push es
push gs

push dx

mov cx, es

mov ax, 0x9000
mov gs, ax

mov word [gs:.TargetSegment], cx
mov word [gs:.TargetBuffer], bx

xor dl, dl
push 0x2B						; Get starting cluster and size (file)
int 0x80

test al, al
jnz .failure					; Check for success

mov ax, si
test ax, ax
jz .success						; Check for empty file

push cx

mov cx, word [gs:.TargetSegment]
mov es, cx

mov bx, word [gs:.TargetBuffer]

push 0x13						; Get current drive
int 0x80

call fat_load_chain				; Load chain into the buffer

pop cx

jmp .success

.failure:

pop dx

xor dl, dl
not dl
jmp .done

.success:

pop dx

xor dl, dl

.done:

pop gs
pop es
pop di
pop si
pop bx
pop ax
iret


.TargetSegment dw 0x0000
.TargetBuffer  dw 0x0000


load_dir:

; *************************************
;     Loads a new working directory
; *************************************

; IN:
; ds:si		-->		Directory name

; OUT:
; dl		-->		0xFF if failure, cleared on success

push ax
push bx
push cx
push si
push di
push es

push dx

mov ax, 0x9000
mov es, ax

mov word [es:.DirName], si

mov dl, 0x01
push 0x2B						; Get starting cluster and size (directory)
int 0x80

test al, al
jnz .failure					; Check for success

mov ax, si

mov word [es:CurrentDirectoryCluster], ax

push 0x13						; Get current drive
int 0x80

call erase_dir_cache			; Erase the directory cache
mov bx, CurrentDirectoryCache	; Load in the current directory cache

test ax, ax						; Check if chain points to 0x0000 (root)
jz .load_root

call fat_load_chain				; Load chain into the buffer

jmp .success

.load_root:

call fat_load_root
jmp .success

.failure:

pop dx

xor dl, dl
not dl
jmp .done

.success:

mov si, word [es:.DirName]

mov di, CurrentDirectoryPath		; Save current path
push 0x2F
int 0x80

pop dx

xor dl, dl

.done:

pop es
pop di
pop si
pop cx
pop bx
pop ax
iret

.DirName	dw	0x0000



get_current_dir:

; *****************************************
;     Returns the current path in ES:DI
; *****************************************

push ax
push si
push ds

mov ax, 0x9000
mov ds, ax

mov si, CurrentDirectoryPath
push 0x27
int 0x80

pop ds
pop si
pop ax
iret


fat_write_entry:

; ************************************************************
;     Writes a new entry into the current directory buffer
; ************************************************************

; IN: DS:SI file name
;     DI = starting cluster
;     AL = attributes
;     BX = raw FAT time
;     ECX = file size
;     DX = raw FAT date

; OUT: Nothing

push eax
push ebx
push ecx
push edx
push si
push di
push ds
push es

mov ax, 0x9000
mov es, ax

mov word [es:.Cluster], di

mov di, .ConvertedName			; Convert to fat name
push 0x17
int 0x80

mov ds, ax

mov byte [.Attributes], al
mov word [.RawTime], bx
mov word [.RawDate], dx
mov dword [.FileSize], ecx

mov di, CurrentDirectoryCache
mov word [.EntryCounter], 0x0000

.find_empty_slot:

inc word [.EntryCounter]

mov ah, byte [es:di]	; Byte from the directory table, first of entry
cmp ah, 0xE5	; Empty entry?
jz .write_entry

mov ah, byte [es:di]	; Byte from the directory table, first of entry
test ah, ah		; End of table?
jz .write_entry

; Skip entry

mov ax, 32
mov di, CurrentDirectoryCache
mul word [.EntryCounter]
add di, ax
jmp .find_empty_slot

.write_entry:

mov si, .ConvertedName

mov cx, 11				; Copy file name
rep movsb

mov al, byte [.Attributes]		; Store attributes
stosb

xor ax, ax				; Erase unused entries
mov cx, 5
rep stosw

mov ax, word [.RawTime]		; Get time
stosw

mov ax, word [.RawDate]		; Get date
stosw

mov ax, word [.Cluster]		; Get cluster
stosw

mov eax, dword [.FileSize]	; Get size
stosd

pop es
pop ds
pop di
pop si
pop edx
pop ecx
pop ebx
pop eax
iret


.EntryCounter	dw	0x0000
.Cluster		dw	0x0000
.FileSize		dd	0x00000000
.RawDate		dw	0x0000
.RawTime		dw	0x0000
.Attributes		db	0x00
.ConvertedName	times 12 db 0x00




fat_get_metadata:

; *****************************************************************
;     Returns the metadata of an entry in the current directory
; *****************************************************************

; IN: DS:SI file/dir name
;     DL = 1 for dir, 0 for file

; OUT:
;      SI = starting cluster, 0x0000 if not found
;      AL = 0x00 if success, 0xFF if not found
;      BX = raw FAT time
;      ECX = file size
;      DX = raw FAT date

push di
push ds
push es

push eax

mov ax, 0x9000
mov es, ax

mov di, .ConvertedName			; Convert to fat name
push 0x17
int 0x80

mov ds, ax

mov byte [.DirectoryFlag], dl
mov byte [.Success], 0x00

mov di, CurrentDirectoryCache
mov word [.EntryCounter], 0x0000

.next_entry:

mov si, .ConvertedName

inc word [.EntryCounter]

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

mov al, byte [di]
and al, 0x10			; Directory?
jnz .directory
cmp byte [.DirectoryFlag], 0x00
jne .skip_entry
jmp .get_metadata
.directory:
cmp byte [.DirectoryFlag], 0x01
jne .skip_entry

.get_metadata:

add di, 11				; Get raw creation time
mov ax, word [es:di]
mov word [.RawTime], ax

add di, 2				; Get raw creation date
mov ax, word [es:di]
mov word [.RawDate], ax

add di, 2				; Retrieve starting cluster
mov ax, word [es:di]
mov word [.Cluster], ax

add di, 2				; Get file size
mov eax, dword [es:di]
mov dword [.FileSize], eax

jmp .done

.skip_entry:

mov ax, 32
mov di, CurrentDirectoryCache
mul word [.EntryCounter]
add di, ax
jmp .next_entry

.not_found:

mov byte [.Success], 0xFF

.done:

pop eax

mov al, byte [.Success]
mov si, word [.Cluster]
mov bx, word [.RawTime]
mov ecx, dword [.FileSize]
mov dx, word [.RawDate]

pop es
pop ds
pop di
ret


.Success		db	0x00
.EntryCounter	dw	0x0000
.Cluster		dw	0x0000
.FileSize		dd	0x00000000
.RawDate		dw	0x0000
.RawTime		dw	0x0000
.DirectoryFlag	db	0x00
.ConvertedName	times 12 db 0x00



directory_scanner:

; **************************************************
;     Scans the content of the current directory
; **************************************************

; IN:
; ax		-->		Directory entry number

; OUT:
; ax		-->		Raw time
; bx		-->		Raw date
; es:di		-->		File name
; ecx		-->		Size
; dh		-->		Directory flag (0x00 if file, 0xFF if directory)
; dl		-->		0xFF if entry not found, 0x00 if found

push si
push di
push ds
push es

mov bx, 0x9000
mov ds, bx

mov word [.EntryNumber], ax
mov word [.TargetBuffer], di
mov ax, es
mov word [.TargetSegment], ax
mov word [.EntryCounter], 0x0000

mov es, bx

mov si, CurrentDirectoryCache

.next_entry:

mov di, .FatNameBuffer
lodsb	; Byte from the directory table, first of entry
dec si
test al, al		; End of table?
jz .failure

mov cx, 11
rep movsb
xor al, al
stosb

; Check for special reserved entries

mov al, byte [si-11]	; Check for deleted entry
cmp al, 0xE5
je .skip_entry

mov al, byte [si]		; Check for a vfat entry (ignore it)
cmp al, 0x0F
je .skip_entry

mov al, byte [si]		; Check for a dir entry
and al, 0x10
jz .no_directory
mov byte [.DirectoryFlag], 0xFF
jmp .directory_check_done
.no_directory:
mov byte [.DirectoryFlag], 0x00
.directory_check_done:

mov ax, word [si+11]	; Get time
mov word [.RawTime], ax

mov ax, word [si+13]	; Get date
mov word [.RawDate], ax

mov eax, dword [si+17]	; Get size
mov dword [.FileSize], eax

mov ax, word [.EntryNumber]
cmp word [.EntryCounter], ax
je .success

inc word [.EntryCounter]

.skip_entry:

add si, 21		; Get to the next entry
jmp .next_entry


.failure:

xor dl, dl
not dl
jmp .done

.success:

mov si, .FatNameBuffer				; Convert name
mov di, word [.TargetBuffer]
mov ax, word [.TargetSegment]
mov es, ax

push 0x2A
int 0x80

xor dl, dl

.done:

mov dh, byte [.DirectoryFlag]
mov ax, word [.RawTime]
mov bx, word [.RawDate]
mov ecx, dword [.FileSize]

pop es
pop ds
pop di
pop si
iret



.DirectoryFlag		db	0x00
.EntryNumber		dw	0x0000
.EntryCounter		dw	0x0000
.TargetBuffer		dw	0x0000
.TargetSegment		dw	0x0000
.RawDate			dw	0x0000
.RawTime			dw	0x0000
.FileSize			dd	0x00000000
.FatNameBuffer		times 12 db 0x00




