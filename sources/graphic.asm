org 0x100
bits 16

push 0x80		; Enter graphics mode
int 0x80

mov cl, 20		; Print pixel
mov ch, 20
mov dl, 4
push 0x81
int 0x80

push 0x18		; Pause
int 0x80

push 0x82		; Exit graphics mode
int 0x80



push 0x00
int 0x80
