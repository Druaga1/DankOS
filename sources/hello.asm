org 0x0100			; Program is loaded at offset 0x0100
bits 16				; 16-bit real mode code

xor si, si
mov di, four_twenty			; test for /420 switch
push 0x08
int 0x80
jc print_420		; If it's present, go ahead

mov si, hello		; Load SI with the pointer to the 0x00 terminated string

push 0x02			; DankOS API function 0x02 is print
int 0x80			; Execute function

push 0x00			; Function 0x00 is terminate execution
int 0x80			; Execute function

print_420:

mov si, blaze_it

push 0x02
int 0x80

push 0x00
int 0x80

hello db 'hello, world!', 0x0A, 0x00
blaze_it db 'aay 420 blaze it!!11', 0x0A, 0x00
four_twenty db '/420', 0x00
