; **** cd command ****

cd_cmd:

mov si, command_line_switches
cmp byte [si], 0x00
je .missing_parameter

push 0x21			; Load directory
int 0x80

test dl, dl			; Check for failure
jnz .failure

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





.missing_parameter_msg	db 'Required parameter missing: directory name.', 0x0A, 0x00
.failure_msg			db 'Directory not found.', 0x0A, 0x00
