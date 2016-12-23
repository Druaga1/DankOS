; ********** CD command ***********

cd_cmd:

; Load the new directory

mov si, command_line_switches
push 0x83
int 0x80

test dl, dl			; Check if the directory was correctly loaded
jz .err

jmp prompt_loop

.err:

mov si, .error_msg
push 0x02
int 0x80

jmp prompt_loop

.error_msg db "Invalid directory.", 0x0A, 0x00
