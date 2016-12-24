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

mov si, .FileName
push 0x02
int 0x80

push 0x03
int 0x80

inc ax
jmp .loop




.FileName		times 12 db 0x00
