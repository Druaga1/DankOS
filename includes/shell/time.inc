; **** Time command ****

time_cmd:

push 0x20				; Read timer
int 0x80

mov byte [.hours], ch	; Save timer values
mov byte [.minutes], cl
mov byte [.seconds], dh

xor eax, eax			; Prepare print integer call: not left aligned, print at least 2 chars
xor dl, dl
mov cl, 2

mov al, byte [.hours]		; Get and print hours
push 0x06
int 0x80

mov al, ':'					; Print separator
push 0x01
int 0x80

mov al, byte [.minutes]		; Get and print minutes
push 0x06
int 0x80

mov al, ':'					; Print separator
push 0x01
int 0x80

mov al, byte [.seconds]		; Get and print seconds
push 0x06
int 0x80

push 0x03				; New line
int 0x80

jmp prompt_loop			; Return to prompt

.hours		db	0x00
.minutes	db	0x00
.seconds	db	0x00
