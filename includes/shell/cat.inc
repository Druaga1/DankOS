; **** cat command ****

cat_cmd:

mov si, command_line_switches
cmp byte [si], 0x00
je .missing_parameter

push es				; Set up target segment:offset
mov ax, word [CatBuffer]
mov es, ax
xor bx, bx

push 0x12			; Load file into buffer
int 0x80
pop es

test dl, dl			; Check for failure
jnz .failure

mov bx, cx

push ds
mov ax, word [CatBuffer]
mov ds, ax

mov byte [bx], 0x00	; Add terminator to the file
xor si, si

push 0x02
int 0x80
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



CatBuffer				dw 0x0000
