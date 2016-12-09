; **** Help command ****

help_cmd:

mov si, .help_msg		; Display help message
push 0x02
int 0x80

jmp prompt_loop			; Return to prompt


.help_msg:

db	"DankOS shell, list of internal commands.", 0x0A
db	0x0A
db	" clear     --     Clears the screen.", 0x0A
db	" help      --     Shows this list.", 0x0A
db	" exit      --     Closes the shell.", 0x0A
db	0x00
