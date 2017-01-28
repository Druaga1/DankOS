; ****************************************************************
;     Prints a sequence of ASCII chars from ESI, length in ECX
; ****************************************************************

ascii_dump:

push ax
push ecx
push esi

.loop:

a32 o32 lodsb					; Byte from ESI
push 0x01
int 0x80
a32 o32 loop .loop				; Loop

.done:

pop esi
pop ecx
pop ax
iret
