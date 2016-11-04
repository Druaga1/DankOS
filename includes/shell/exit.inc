; **** Exit command ****

exit_cmd:

xor eax, eax				; Exit code = 0
int 0x40
