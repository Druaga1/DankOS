fat_browser_get_cluster:

; ***************************************************************************************
;     Returns the starting cluster of the file chain of the file in DS:SI in fat name
; ***************************************************************************************

; IN: DS:DI file fat name, DH = 1 if looking for dir, 0 if file
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
mov byte [es:.DirectoryFlag], dh

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

cmp byte [es:.DirectoryFlag], 0x00
je .check_if_file

.check_if_directory:

mov ah, byte [es:di]
and ah, 0x10
cmp ah, 0x10		; Test for directory flag
jne .skip_entry		; If not a dir, skip
jmp .retrieve_cluster

.check_if_file:

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
pop bx
ret

.DirectoryFlag	db	0x00
.EntryCounter	dw	0x0000
.Cluster		dw	0x0000
.FileName		dw	0x0000
.FileSize		dd	0x00000000
