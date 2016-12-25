; **** ls command ****

ls_cmd:

mov di, .FileName

push 0x13				; Get current drive
int 0x08

xor ax, ax

.loop:
push 0x28
int 0x80

jc prompt_loop			; Return to prompt

cmp dl, 0xFF
je .add_brackets

mov si, .FileName
push 0x02
int 0x80

.done:

push 0x03
int 0x80

inc ax
jmp .loop

.add_brackets:

mov al, '['
push 0x01
int 0x80
mov si, .FileName
push 0x02
int 0x80
mov al, ']'
push 0x01
int 0x80
jmp .done



.FileName		times 13 db 0x00
