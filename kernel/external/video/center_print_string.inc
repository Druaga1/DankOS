; *************************************************************************
;     Prints a 0x00 terminated string in DS:SI at the center of the row
; *************************************************************************

center_print_string:

push ax
push bx
push cx
push dx

push 0x09
int 0x80				; Find length of string

mov ax, 80				; 80 - length
sub ax, cx
xor dx, dx
mov bx, 2
div bx					; divide / 2
mov dx, ax
push 0x0D
int 0x80
mov ah, dl
push 0x0E
int 0x80
push 0x02
int 0x80

.done:

pop dx
pop cx
pop bx
pop ax
iret
