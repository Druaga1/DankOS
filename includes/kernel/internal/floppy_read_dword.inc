floppy_read_dword:

; ************************************************************
;     Reads a dword from a specific location from a floppy
; ************************************************************

; IN:
; EBX = Dword address
; DL = Drive number

; OUT:
; EAX = Dword read

push ebx
push cx					; Save regs

add ebx, 3				; Read last to first byte, since it's little endian

mov cx, 4				; Loop 4 times

.loop:
shl eax, 8				; Rotate EAX left
push 0x24				; Read byte call
int 0x80
dec ebx					; Next byte
loop .loop				; Loop!

pop cx					; Restore regs
pop ebx

iret
