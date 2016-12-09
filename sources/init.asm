; ** DankOS shell **

org 0x0100

bits 16							; 16-bit real mode

mov si, intro					; Print intro
push 0x02
int 0x80

prompt_loop:

mov si, prompt					; Draw prompt
push 0x02
int 0x80
mov bx, 200						; Limit input to 200 characters
mov di, prompt_input			; Point to local buffer
push 0x10
int 0x80						; Input string
push 0x03
int 0x80						; New line
cmp byte [prompt_input], 0x00	; If no input, restart loop
je prompt_loop

; ***** Check for internal commands *****

mov si, prompt_input

mov di, exit_msg				; Exit command
push 0x08
int 0x80
jc exit_cmd

mov di, clear_msg				; Clear command
push 0x08
int 0x80
jc clear_cmd





push 0x13
int 0x80						; Load current drive in DL
mov si, prompt_input			; Prepare SI for start process function
xor eax, eax					; Reset EAX
push 0x14
int 0x80						; Try to start new process
cmp eax, 0xFFFFFFFF				; If fail, print error message
jne prompt_loop					; Otherwise restart the loop

mov si, not_found
push 0x02
int 0x80
jmp prompt_loop

data:

intro		db	0x0A
			db	0x0A, 'DankOS shell, welcome!'
			db	0x0A, 0x0A, 0x00

prompt		db	'>> ', 0x00

not_found	db	'Invalid command or file name.', 0x0A, 0x00

prompt_input	times 201 db 0x00




internal_commands:

exit_msg			db	'exit', 0x00
clear_msg			db	'clear', 0x00




%include 'includes/shell/exit.inc'
%include 'includes/shell/clear.inc'
