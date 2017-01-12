path_converter:

; *****************************************************
;     Converts a relative path into an absolute one
; *****************************************************

; IN: DS:SI = Relative path to convert
; OUT: ES:DI = Converted path

push ax					; Save used registers
push bx
push cx
push dx
push si
push di
push gs

mov ax, KernelSpace			; GS to kernel
mov gs, ax

mov word [gs:.TargetBuffer], di			; Save address of target location

; Check if the path is absolute

lodsb
cmp al, '/'
je .main
dec si

push 0x2E			; Dump the pwd in the target buffer
int 0x80

push 0x2D			; Get to end of string
int 0x80



.main:

mov bx, .DirectoryName		; Put a temporary buffer in BX to check for . or .. entries

.name_loop:

lodsb
cmp al, '/'
je .directory
test al, al
jz .directory
mov byte [gs:bx], al
inc bx
jmp .name_loop

.directory:

mov byte [gs:bx], 0x00	; Add terminator to buffer

push si				; Save these registers
push di
push ds
push es

mov ax, KernelSpace		; Point es and ds to kernel
mov ds, ax
mov es, ax

; Check for reserved entries (. and ..)

mov si, .DirectoryName
mov di, .DotEntry

push 0x08		; Compare strings
int 0x80

test dl, dl
jz .dot_entry

mov di, .DotDotEntry

push 0x08
int 0x80

test dl, dl
jz .dot_dot_entry



; If not a reserved entry			--- CASE 1

pop es
pop ds
pop di
pop si

mov bx, .DirectoryName

; check if root dir

mov ax, di
dec ax
cmp ax, word [gs:.TargetBuffer]
je .no_slash

mov al, '/'			; Add slash
stosb

.no_slash:

.copy_to_buffer_loop:

mov al, byte [gs:bx]
test al, al
jz .not_reserved_done
inc bx
stosb
jmp .copy_to_buffer_loop

.not_reserved_done:

mov byte [es:di], 0x00			; Put a terminator, just in case

mov al, byte [ds:(si-1)]
test al, al			; If end of string, done
jz .done

jmp .main



.dot_entry:

; If a dot (.) entry				--- CASE 2

pop es
pop ds
pop di
pop si

mov al, byte [ds:(si-1)]
test al, al			; If end of string, done
jz .done

jmp .main




.dot_dot_entry:

; If a dot dot (..) entry			--- CASE 3

pop es
pop ds
pop di
pop si

.remove_last_dir_loop:

; check if root dir

mov al, byte [es:di]
mov byte [es:di], 0x00
cmp al, '/'
je .remove_dir_done

mov ax, di
dec ax
cmp ax, word [gs:.TargetBuffer]
je .remove_dir_done

dec di
jmp .remove_last_dir_loop

.remove_dir_done:

mov al, byte [ds:(si-1)]
test al, al			; If end of string, done
jz .done

jmp .main







.done:

pop gs
pop di
pop si
pop dx
pop cx
pop bx
pop ax

push 0x15			; Convert to uppercase
int 0x80

ret



.DirectoryName		times 13 db 0x00
.DotEntry			db '.', 0x00
.DotDotEntry		db '..', 0x00
.TargetBuffer		dw 0x0000
