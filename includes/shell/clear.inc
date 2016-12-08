; ** Clear command **

clear_cmd:

push 0x0A
int 0x80			; Reinitialise screen
jmp prompt_loop		; Return to prompt
