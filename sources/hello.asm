org 0x0100			; Program is loaded at offset 0x0100
bits 16				; 16-bit real mode code

mov di, buffer
mov bx, 128

push 0x10			; DankOS API function 0x02 is print
int 0x80			; Execute function

push 0x03
int 0x80

mov si, buffer
mov di, target

push 0x85
int 0x80

mov si, target
push 0x02
int 0x80

push 0x03
int 0x80

push 0x00			; Function 0x00 is terminate execution
int 0x80			; Execute function

buffer times 129 db 0x00
target times 129 db 0x00
