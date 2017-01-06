org 0x0100			; Program is loaded at offset 0x0100
bits 16				; 16-bit real mode code


push 0x80			; Enter graphics mode
int 0x80

mov bx, 0x3020
mov dl,0x2
mov cx,0x1010

push 0x83			;Draw Line
int 0x80


push 0x18			; Pause
int 0x80

push 0x82			; Exit graphics mode
int 0x80

push 0x00			; Return to the command line
int 0x80

