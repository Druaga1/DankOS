org 0x0100			; Program is loaded at offset 0x0100
bits 16				; 16-bit real mode code



mov si, file
mov bx, buffer
push 0x80
int 0x80


mov si, buffer
push 0x02
int 0x80



push 0x00
int 0x80


file db '/test/hallo.txt', 0x00
file2 db 'hello.txt', 0x00
file3 db 'test/weed/weed.420', 0x00

buffer times 200 db 0x00
