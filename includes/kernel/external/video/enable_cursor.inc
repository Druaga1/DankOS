; ********************************************
;     Enables (shows) the text mode cursor
; ********************************************

enable_cursor:

push ax
push ds

mov ax, KernelSpace
mov ds, ax

call draw_cursor
mov byte [CursorStatus], 0x01

pop ds
pop ax

iret
