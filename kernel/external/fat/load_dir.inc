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

mov ax, KernelSpace
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
call path_converter

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
