org 0x0100			; Program is loaded at offset 0x0100
bits 16				; 16-bit real mode code

mov ax,261
push 0x22
int 0x80

push 0x00			; Function 0x00 is terminate execution
int 0x80			; Execute function

