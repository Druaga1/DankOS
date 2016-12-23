; ** DankOS shell **

org 0x0100

bits 16							; 16-bit real mode

mov si, intro					; Print intro
push 0x02
int 0x80

prompt_loop:

mov di, current_dir
push 0x82						; Get current directory
int 0x80

mov si, current_dir				; Draw current directory
push 0x02
int 0x80

mov si, prompt					; Draw prompt
push 0x02
int 0x80
mov bx, 0xFF					; Limit input to 0xFF characters
mov di, prompt_input			; Point to local buffer
push 0x10
int 0x80						; Input string
push 0x03
int 0x80						; New line
cmp byte [prompt_input], 0x00	; If no input, restart loop
je prompt_loop

; *** Extract command line switches from the input ***

extract_switches:

mov si, prompt_input			; Setup destination and source indexes
mov di, command_line_switches

.find_space_loop:
lodsb							; Byte from SI
cmp al, ' '						; Is it space?
je .get_switches				; If it is, save switches
test al, al						; Is it 0x00?
jz .no_switches					; If it is, write a 0x00 in the buffer, and quit
jmp .find_space_loop			; Otherwise loop

.get_switches:
mov byte [si-1], 0x00			; Add a terminator to the input

.get_switches_loop:
lodsb							; Byte from SI
stosb							; Save it in DI
test al, al						; Is it 0x00?
jz .done						; If yes, we're done
jmp .get_switches_loop			; Otherwise loop

.no_switches:
mov byte [di], 0x00

.done:

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

mov di, help_msg				; Help command
push 0x08
int 0x80
jc help_cmd

mov di, cd_msg					; Cd command
push 0x08
int 0x80
jc cd_cmd





push 0x13
int 0x80						; Load current drive in DL
mov si, prompt_input			; Prepare SI for start process function
mov di, command_line_switches	; Prepare to pass the switches
push 0x14
int 0x80						; Try to start new process
cmp eax, 0xFFFFFFFF				; If fail, print error message
jne prompt_loop					; Otherwise restart the loop

mov si, not_found
push 0x02
int 0x80
jmp prompt_loop

data:

intro		db	0x0A, 'DankOS shell, welcome!'
			db	0x0A, 0x0A, 0x00

prompt		db	'# ', 0x00

not_found	db	'Invalid command or file name.', 0x0A, 0x00

prompt_input	times 0x100 db 0x00

command_line_switches	times 0x100 db 0x00

current_dir		times 128 db 0x00


internal_commands:

exit_msg			db	'exit', 0x00
clear_msg			db	'clear', 0x00
help_msg			db	'help', 0x00
cd_msg				db	'cd', 0x00




%include 'includes/shell/exit.inc'
%include 'includes/shell/clear.inc'
%include 'includes/shell/help.inc'
%include 'includes/shell/cd.inc'
