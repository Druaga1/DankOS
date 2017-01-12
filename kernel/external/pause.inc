pause:

;	Pause the system until a key is pressed.

push ax

xor ah, ah
int 0x16

pop ax
iret
