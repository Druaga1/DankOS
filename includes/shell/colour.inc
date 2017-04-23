; **** Colour command ****

colour_cmd:

mov si, command_line_switches
cmp byte [si], 0x00
je .missing_parameter

push 0x17
int 0x80

mov bl, al

push 0x04
int 0x80

mov ah, al
mov al, bl

push 0x11
int 0x80

jmp prompt_loop			; Return to prompt



.missing_parameter:

mov si, .missing_parameter_msg
push 0x02
int 0x80
jmp prompt_loop





.missing_parameter_msg	db 'Required parameter missing: palette 0-255.', 0x0A, 0x00
