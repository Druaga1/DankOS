org 0x0100			; Program is loaded at offset 0x0100
bits 16				; 16-bit real mode code

mov si, length_msg
push 0x02
int 0x80			;Print the string
push 0x07
int 0x80			;Get the length

mov al, 0x0A
push 0x01
int 0x80
	
mov bx, ax
mov si, frequency_msg
push 0x02
int 0x80			;Print the string
push 0x07
int 0x80			;Get the frequency

push 0x03
int 0x80

push 0x22
int 0x80

push 0x00			; Function 0x00 is terminate execution
int 0x80			; Execute 


frequency_msg		db	'Frequency: ', 0x00
length_msg			db	'Length: ', 0x00

