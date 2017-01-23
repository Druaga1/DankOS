pause:

;	Pause the system until a key is pressed.

push ax

push 0x1C
int 0x80

pop ax
iret
