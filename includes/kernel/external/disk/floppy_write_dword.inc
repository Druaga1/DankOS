floppy_write_dword:

; *********************************************************
;     Writes a dword to a specific location on a floppy
; *********************************************************

; IN:
; EAX = Dword to write
; EBX = Dword address
; DL = Drive number

push eax
push ebx
push cx					; Save regs

add ebx, 3				; Write last to first byte, since it's little endian

mov cx, 4				; Loop 4 times

.loop:
push 0x32				; Write byte call
int 0x80
shr eax, 8				; Rotate EAX right
dec ebx					; Next byte
loop .loop				; Loop!

pop cx					; Restore regs
pop ebx
pop eax

iret
