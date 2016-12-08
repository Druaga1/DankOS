org 0x0100			; Program is loaded at offset 0x0100
bits 16				; 16-bit real mode code

mov si, hello		; Load SI with the pointer to the 0x00 terminated string

push 0x02			; DankOS API function 0x02 is print
int 0x80			; Execute function

push 0x00			; Function 0x00 is terminate execution
int 0x80			; Execute function

hello db 'hello, world!', 0x0A, 0x00
