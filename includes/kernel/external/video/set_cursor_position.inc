; ***************************************************************
;     Sets up a new current position for the text mode cursor
; ***************************************************************

set_cursor_position:

; IN: AH = cursor X
;     AL = cursor Y

push ax
push bx
push cx
push dx
push ds

mov bx, KernelSpace
mov ds, bx

cmp byte [CursorStatus], 0x01
jne .main
call clear_cursor		; Clear cursor

.main:

push ax						; Save AX
xor ah, ah					; Clear high 8
mov bx, 160					; Multiply Y * 160
mul bx
mov word [CursorLocation], ax	; Save for now
pop ax						; Restore AX
xor al, al					; Clear low 8
shr ax, 8					; Shift right 8
add ax, ax					; Multiply X * 2
add word [CursorLocation], ax	; Add

cmp byte [CursorStatus], 0x01
jne .done
call draw_cursor		; Draw cursor back

.done:

pop ds
pop dx
pop cx
pop bx
pop ax

iret
