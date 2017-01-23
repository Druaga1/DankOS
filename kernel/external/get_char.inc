; ****************************************
;     Gets a char from keyboard (stub)
; ****************************************

; Returns: AL = ASCII char

get_char:

push ds
push ax

mov ax, KernelSpace
mov ds, ax

xor ax, ax
int 0x16

mov byte [.ReturnASCII], al

pop ax

mov al, byte [.ReturnASCII]

pop ds

iret

.ReturnASCII		db	0x00
