; **********************************************
;     Prints a 0x00 terminated string in ESI
; **********************************************

print_string:

push ax
push esi

.loop:

a32 o32 lodsb					; Byte from ESI
test al, al						; Is 0x00?
jz .done						; If yes, done
push 0x01
int 0x80
jmp .loop						; Loop

.done:

pop esi
pop ax
iret
