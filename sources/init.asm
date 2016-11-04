; ** DankOS shell **

bits 16							; 16-bit real mode

mov ax, 128						; Allocate 128 bytes of memory for stack
int 0x59

cli								; Prepare stack
mov ss, cx
mov sp, 128
sti

mov si, intro					; Print intro
int 0x42

int 0x4C						; Enable cursor

prompt_loop:

mov si, prompt					; Draw prompt
int 0x42
mov bx, 200						; Limit input to 200 characters
mov di, prompt_input			; Point to local buffer
int 0x50						; Input string
int 0x43						; New line
cmp byte [prompt_input], 0x00	; If no input, restart loop
je prompt_loop

; ***** Check for internal commands *****

mov si, prompt_input

mov di, exit_msg				; Exit command
int 0x48
jc exit_cmd






int 0x53						; Load current drive in DL
mov si, prompt_input			; Prepare SI for start process function
xor eax, eax					; Reset EAX
int 0x54						; Try to start new process
cmp eax, 0xFFFFFFFF				; If fail, print error message
jne prompt_loop					; Otherwise restart the loop

mov si, not_found
int 0x42
jmp prompt_loop

data:

intro		db	0x0A
			db	0x0A, 'DankOS shell, welcome!'
			db	0x0A, 0x0A, 0x00

prompt		db	'>> ', 0x00

not_found	db	'File not found.', 0x0A, 0x00

prompt_input	times 201 db 0x00




internal_commands:

exit_msg			db	'exit', 0x00




%include 'includes/shell/exit.inc'
