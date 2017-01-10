; **** cat command ****

cat_cmd:

mov si, command_line_switches
cmp byte [si], 0x00
je .missing_parameter

push es				; Set up target segment:offset
mov ax, word [FileBuffer]
mov es, ax
xor bx, bx

push 0x12			; Load file into buffer
int 0x80
pop es

test dl, dl			; Check for failure
jnz .failure

push ds
mov ax, word [FileBuffer]
mov ds, ax

xor si, si

.loop:
lodsb
push 0x01
int 0x80
loop .loop

pop ds

jmp prompt_loop


.missing_parameter:

mov si, .missing_parameter_msg
push 0x02
int 0x80
jmp prompt_loop

.failure:

mov si, .failure_msg
push 0x02
int 0x80
jmp prompt_loop





.missing_parameter_msg	db 'Required parameter missing: file name.', 0x0A, 0x00
.failure_msg			db 'File not found.', 0x0A, 0x00
