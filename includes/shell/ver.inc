; **** Ver command ****

ver_cmd:

mov edi, .version

push 0x87		; Get version number
int 0x80

mov esi, .version

push 0x02
int 0x80
push 0x03
int 0x80

jmp prompt_loop			; Return to prompt

.version times 16 db 0x00
