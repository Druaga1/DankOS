floppy_read_word:

; ***********************************************************
;     Reads a word from a specific location from a floppy
; ***********************************************************

; IN:
; EBX = Word address
; DL = Drive number

; OUT:
; AX = Word read

inc ebx					; Read last to first byte, since it's little endian
push 0x24				; Read byte call
int 0x80
mov ah, al				; Put the higher byte in AH
dec ebx					; Next byte
push 0x24				; Read byte call
int 0x80

iret
