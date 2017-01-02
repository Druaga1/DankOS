; **** Debug command ****

debug_cmd:

mov si, .test
mov di, 0x05
mov al, 0x00
xor dx, dx
xor bx, bx
mov cx, 123

push 0x30
int 0x80

jmp prompt_loop			; Return to prompt







.test db "test.txt", 0x00
