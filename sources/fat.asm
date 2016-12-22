org 0x0100			; Program is loaded at offset 0x0100
bits 16				; 16-bit real mode code

push 0x07
int 0x80

push 0x03
int 0x80

mov bx, buffer
mov dl, 0x00
push 0x80
int 0x80




mov si, buffer

mov cx, 1024

loops:

xor eax, eax
lodsb
push 0x05
int 0x80

mov al, ' '
push 0x01
int 0x80

loop loops







push 0x00
int 0x80


times 512-($-$$) db 0x00


buffer times 5000 db 0x00
