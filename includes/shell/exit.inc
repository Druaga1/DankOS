; **** Exit command ****

exit_cmd:

xor eax, eax				; Exit code = 0
push 0x00
int 0x80
