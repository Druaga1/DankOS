; *********************************************
;     Disables (hides) the text mode cursor
; *********************************************

disable_cursor:

push ax
push ds

mov ax, KernelSpace
mov ds, ax

call clear_cursor
mov byte [CursorStatus], 0x00

pop ds
pop ax

iret
