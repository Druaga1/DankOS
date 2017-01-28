; ************************************************************
;     Returns the current position of the text mode cursor
; ************************************************************

get_cursor_position:

; OUT: AH = cursor X
;      AL = cursor Y

push bx
push cx
push dx
push ds

mov ax, KernelSpace
mov ds, ax

mov ax, word [CursorLocation]
mov bx, 160					; Divide AX / 160
xor dx, dx
div bx
xor ah, ah					; Clear AH
push ax						; Push result, for now
mov ax, dx					; Load MOD
mov bx, 2					; Divide MOD / 2
xor dx, dx
div bx
mov dx, ax					; Result in DX
pop ax						; Restore AX
mov ah, dl					; Move AH

pop ds
pop dx
pop cx
pop bx

iret
