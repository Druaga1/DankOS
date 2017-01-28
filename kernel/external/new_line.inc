; *****************************************************
;     Moves the cursor to the next line (like a \n)
; *****************************************************

new_line:

push ax

mov al, 0x0A
push 0x01
int 0x80

pop ax
iret
