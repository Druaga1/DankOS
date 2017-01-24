cut_string:

; Cuts a string at the desired spot, and adds a terminator

; IN:		DS:ESI = string to cut
;			BL = character to cut at

; OUT:		DS:ESI = string cut
;			DS:EBX = starting position of the second part of the string

push ax
push esi

.loop:
a32 o32 lodsb
cmp al, bl
je .cut_string
test al, al
jz .fail
jmp .loop

.cut_string:
dec esi
xor al, al
mov byte [ds:esi], al
inc esi
jmp .done

.fail:
dec esi

.done:
mov ebx, esi
pop esi
pop ax
iret
