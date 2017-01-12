; *********************************
;     Gets metadata from a file
; *********************************

; IN: DS:SI = Relative path
;     DL = 1 for dir, 0 for file

; OUT:
;      SI = starting cluster, 0x0000 if not found
;      AL = 0x00 if success, 0xFF if not found
;      BX = raw FAT time
;      ECX = file size
;      DX = raw FAT date

ping_file:

push di
push ds
push es

mov ax, KernelSpace
mov es, ax

mov byte [es:.DirectoryFlag], dl

; Check for root

lodsb
cmp al, '/'
je .load_root
dec si

; Fetch file name

.loop:
mov di, .FileName
mov cx, 12

.get_file_name:
lodsb
cmp al, '/'
je .fetch_entry
stosb
test al, al
jz .fetch_target_entry
loop .get_file_name

.fetch_entry:
xor al, al
stosb
mov dl, 1			; We're looking for a dir
push si
push ds
mov ax, KernelSpace
mov ds, ax
mov si, .FileName
call fat_get_metadata
mov word [.Cluster], si
pop ds
pop si
cmp al, 0xFF
je .restore

; Now temporarily load the fetched dir in the buffer

call erase_dir_cache
mov ax, word [es:.Cluster]
mov bx, CurrentDirectoryCache
push 0x13
int 0x80
test ax, ax
jz .load_root
call fat_load_chain
jmp .loop
.load_root:
call fat_load_root
jmp .loop

.fetch_target_entry:
mov dl, byte [es:.DirectoryFlag]
push ax
mov ax, KernelSpace
mov ds, ax
pop ax
mov si, .FileName
call fat_get_metadata

; Now restore the old current directory in the buffer

.restore:

push ax
push bx
push cx
push dx
push si
push di

push 0x13
int 0x80
call erase_dir_cache
mov ax, word [CurrentDirectoryCluster]
test ax, ax
jz .load_root_end
mov bx, CurrentDirectoryCache
call fat_load_chain
jmp .load_dir_done
.load_root_end:
call fat_load_root
.load_dir_done:

pop di
pop si
pop dx
pop cx
pop bx
pop ax


.done:

pop es
pop ds
pop di
iret



.DirectoryFlag		db	0x00
.Cluster			dw	0x0000
.FileName			times	12	db 0x00
