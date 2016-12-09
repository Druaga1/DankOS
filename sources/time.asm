org 0x0100			; Program is loaded at offset 0x0100
bits 16				; 16-bit real mode code

push 0x21			; Print clock
int 0x80

push 0x03			; New line
int 0x80

push 0x00			; Return
int 0x80
