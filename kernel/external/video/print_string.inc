; ************************************************
;     Prints a 0x00 terminated string in DS:SI
; ************************************************

print_string:

push ax
push si

.loop:

lodsb							; Byte from SI
test al, al						; Is 0x00?
jz .done						; If yes, done
push 0x01
int 0x80
jmp .loop						; Loop

.done:

pop si
pop ax
iret
