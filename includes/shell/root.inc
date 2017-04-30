; **** Root command ****

root_cmd:

mov si, command_line_switches
cmp byte [si], 0x00
je .missing_parameter

push 0x04       ; String to integer
int 0x80

mov dl, al
push 0x29       ; Set current drive
int 0x80

jmp prompt_loop			; Return to prompt



.missing_parameter:

mov si, .missing_parameter_msg
push 0x02
int 0x80
jmp prompt_loop





.missing_parameter_msg	db 'Required parameter missing: new root (integer).', 0x0A, 0x00
