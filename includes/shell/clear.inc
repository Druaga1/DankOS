; ** Clear command **

clear_cmd:

int 0x4A			; Reinitialise screen
jmp prompt_loop		; Return to prompt
