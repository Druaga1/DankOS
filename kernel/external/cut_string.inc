cut_string:

; Cuts a string at the first space (0x20), and adds a terminator

; IN:		DS:SI = string to cut

; OUT:		DS:SI = string cut
;			DS:BX = starting position of the second part of the string

push ax
push si

.loop:
lodsb
cmp al, ' '
je .cut_string
test al, al
jz .fail
jmp .loop

.cut_string:
dec si
xor al, al
mov byte [ds:si], al
inc si
jmp .done

.fail:
dec si

.done:
mov bx, si
pop si
pop ax
iret
