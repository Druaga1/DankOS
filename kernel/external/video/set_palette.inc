; *****************************************
;     Sets up a new (text mode) palette
; *****************************************

; IN: AH --> character attributes
;     AL --> cursor attributes

set_palette:

push bx
push ds

mov bx, KernelSpace
mov ds, bx

mov byte [CharAttributes], ah
mov byte [CursorAttributes], al

cmp byte [CursorStatus], 0x01
jne .done
call draw_cursor

.done:

pop ds
pop bx

iret
