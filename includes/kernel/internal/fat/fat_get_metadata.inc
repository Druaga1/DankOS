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

mov ax, KernelSpace
mov es, ax

mov di, .ConvertedName			; Convert to fat name
call string_to_fat_name

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
