; **** Ver command ****

ver_cmd:

push 0x87		; Get version number
int 0x80

xor cl, cl
xor dl, dl

push eax
shr eax, 24
push 0x06
int 0x80
mov al, '.'
push 0x01
int 0x80
pop eax

push eax
shr eax, 16
and eax, 0x000000FF
push 0x06
int 0x80
mov al, '.'
push 0x01
int 0x80
pop eax

push eax
shr eax, 8
and eax, 0x000000FF
push 0x06
int 0x80
mov al, '.'
push 0x01
int 0x80
pop eax

and eax, 0x000000FF
push 0x06
int 0x80

push 0x03
int 0x80

jmp prompt_loop			; Return to prompt
