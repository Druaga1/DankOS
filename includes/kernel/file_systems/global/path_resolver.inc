; ******************************************************************************
;     Resolves a directory or file path to the starting cluster of the chain
; ******************************************************************************

fat_path_resolver:

; IN: DS:SI path of the file/dir
;     DL = Drive number
;     DH = 0 if the caller wants a file entry, 1 for a directory entry

; OUT: AX = starting cluster, 0x0000 if failure, file size in ECX

push bx
push dx
push si
push di
push ds
push es

mov ax, 0x9000							; ES to kernel space
mov es, ax

mov di, .ConvertedPath			; Convert to absolute path
call fat_path_converter

mov ds, ax					; DS to kernel space

mov byte [.TargetDrive], dl			; Save drive number
mov byte [.DirectoryFlag], dh		; Save the directory flag

mov si, .ConvertedPath

; Check if the path only points to the root

push si
lodsw
pop si
cmp ax, 0x002F		; Check for / + 0x00
je .failure

inc si				; Move pointer after the initial /

; Load the root directory

mov dl, byte [.TargetDrive]
call fat_load_root

; Main path resolver loop

.loop:
mov di, .ExtractedName

mov cx, 12		; Max 12 characters

.name_loop:
lodsb
cmp al, '/'
je .load_directory
test al, al
jz .file_name
stosb
loop .name_loop

jmp .failure	; If overflow, fail

.load_directory:

mov al, 0x00	; Add terminator
stosb

push si

mov si, .ExtractedName
mov di, .ConvertedName
push 0x17
int 0x80

mov si, .ConvertedName
mov dh, 1				; Directory flag
call fat_browser_get_cluster

pop si

test ax, ax
jz .failure

mov bx, CurrentDirectoryCache
mov dl, byte [.TargetDrive]
call fat12_load_chain

jmp .loop

.file_name:

mov al, 0x00
stosb

mov si, .ExtractedName
mov di, .ConvertedName
push 0x17
int 0x80

mov si, .ConvertedName
mov dh, byte [.DirectoryFlag]	; forward flag to subroutine
call fat_browser_get_cluster

test ax, ax
jz .failure

jmp .success

.failure:

xor ax, ax

.success:

pop es
pop ds
pop di
pop si
pop dx
pop bx
ret




.ConvertedName		times 12 db 0x00
.ExtractedName		times 13 db 0x00
.TargetDrive		db	0x00
.DirectoryFlag		db	0x00
.ConvertedPath		times 129 db 0x00
