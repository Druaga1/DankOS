floppy_write_word:

; ********************************************************
;     Writes a word to a specific location on a floppy
; ********************************************************

; IN:
; AX = Word to write
; EBX = Word address
; DL = Drive number

push ax

inc ebx					; Write last to first byte, since it's little endian
push 0x32				; Write byte call
int 0x80
mov al, ah
dec ebx					; Next byte
push 0x32				; Write byte call
int 0x80

pop ax

iret
