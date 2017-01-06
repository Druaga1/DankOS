; ** DankOS shell **

org 0x0100

bits 16							; 16-bit real mode

mov ax, 8192					; Reserve an 8kb segment of memory for cat
push 0x19
int 0x80
mov word [CatBuffer], cx		; Save segment

mov si, intro					; Print intro
push 0x02
int 0x80

prompt_loop:

mov di, CurrentDir
push 0x2E						; Get current dir
int 0x80
mov si, CurrentDir
push 0x02						; Print current dir
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
push 0x27
int 0x80
jmp .done

.no_switches:
mov byte [di], 0x00

.done:

; ***** Check for internal commands *****

mov si, prompt_input

mov di, exit_msg				; Exit command
push 0x08
int 0x80
test dl, dl
jz exit_cmd

mov di, clear_msg				; Clear command
push 0x08
int 0x80
test dl, dl
jz clear_cmd

mov di, help_msg				; Help command
push 0x08
int 0x80
test dl, dl
jz help_cmd

mov di, ls_msg					; Ls command
push 0x08
int 0x80
test dl, dl
jz ls_cmd

mov di, cat_msg					; Cat command
push 0x08
int 0x80
test dl, dl
jz cat_cmd

mov di, cd_msg					; Cd command
push 0x08
int 0x80
test dl, dl
jz cd_cmd

mov di, time_msg				; Time command
push 0x08
int 0x80
test dl, dl
jz time_cmd

mov di, debug_msg				; Debug command
push 0x08
int 0x80
test dl, dl
jz debug_cmd

mov di, image_msg				; Image command
push 0x08
int 0x80
test dl, dl
jz image_cmd





mov si, prompt_input			; Prepare SI for start process function
mov di, command_line_switches	; Prepare to pass the switches
push 0x14
int 0x80						; Try to start new process

cmp eax, 0xFFFFFFFF				; If fail, add .bin and try again
jne prompt_loop					; Otherwise restart the loop


; Try to add a .bin and load the file again

add_bin:

push 0x09
int 0x80
cmp cx, 12
jg invalid_command

mov di, bin_added_buffer

push 0x27
int 0x80

mov si, bin_added_buffer
.loop:
mov al, byte [ds:si]
test al, al
jz .add_bin
inc si
jmp .loop

.add_bin:
mov di, si
mov si, bin_msg
push 0x27
int 0x80

; Try to load again

mov si, bin_added_buffer		; Prepare SI for start process function
mov di, command_line_switches	; Prepare to pass the switches
push 0x14
int 0x80						; Try to start new process
cmp eax, 0xFFFFFFFF				; If fail, print error message
jne prompt_loop					; Otherwise restart the loop

invalid_command:

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

bin_added_buffer	times 13 db 0x00

command_line_switches	times 0x100 db 0x00

bin_msg		db	'.bin', 0x00

CurrentDir	times 130 db 0x00


internal_commands:

exit_msg			db	'exit', 0x00
clear_msg			db	'clear', 0x00
help_msg			db	'help', 0x00
ls_msg				db	'ls', 0x00
cat_msg				db	'cat', 0x00
cd_msg				db	'cd', 0x00
time_msg			db	'time', 0x00
debug_msg			db	'debug', 0x00
image_msg			db	'image', 0x00




%include 'includes/shell/exit.inc'
%include 'includes/shell/clear.inc'
%include 'includes/shell/help.inc'
%include 'includes/shell/ls.inc'
%include 'includes/shell/cat.inc'
%include 'includes/shell/cd.inc'
%include 'includes/shell/time.inc'
%include 'includes/shell/debug.inc'
%include 'includes/shell/image.inc'
