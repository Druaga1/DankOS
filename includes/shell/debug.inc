; **** Debug command ****

debug_cmd:

mov ebx, 511
mov eax, 0x05060708
xor dl, dl

push 0x34
int 0x80

jmp prompt_loop			; Return to prompt
