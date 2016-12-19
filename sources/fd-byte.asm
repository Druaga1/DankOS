org 0x0100			; Program is loaded at offset 0x0100
bits 16				; 16-bit real mode code

mov si, drive_number_msg
push 0x02
int 0x80

push 0x07
int 0x80

mov dl, al

mov si, byte_address_msg
push 0x02
int 0x80

push 0x07
int 0x80

mov ebx, eax

xor eax, eax

push 0x24
int 0x80

push 0x03
int 0x80

push 0x06
int 0x80

push 0x03
int 0x80

push 0x00
int 0x80

drive_number_msg db "Drive number to read from: ", 0x00
byte_address_msg db 0x0A, "Byte address: ", 0x00
