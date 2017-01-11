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

mov ax, KernelSpace
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
