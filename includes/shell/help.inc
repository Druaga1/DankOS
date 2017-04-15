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
db	" ls/dir    --     Lists the files in the drive.", 0x0A
db	" cd        --     Changes the present working directory.", 0x0A
db	" cat       --     Displays the content of a file.", 0x0A
db	" time      --     Displays the current time.", 0x0A
db	" image     --     Displays the content of a DankOS formatted image file.", 0x0A
db	" ver       --     Shows the current version of the kernel.", 0x0A
db	" help      --     Shows this list.", 0x0A
db	" exit      --     Closes the shell.", 0x0A
db	0x00
