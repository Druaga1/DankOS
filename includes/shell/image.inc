; **** image command ****

image_cmd:

	mov si, command_line_switches
	cmp byte [si], 0x00
	je .missing_parameter

	push es				; Set up target segment:offset
	mov ax, word [CatBuffer]
	mov es, ax
	xor bx, bx

	push 0x12			; Load file into buffer
	int 0x80
	pop es

	test dl, dl			; Check for failure
	jnz .failure

	mov bx, cx

	push ds
	mov ax, word [CatBuffer]
	mov ds, ax

	mov byte [bx], 0xF0	; Add terminator to the file
	xor si, si
	push ax
	push 0x80		; Enter graphics mode
	int 0x80
	

.loop:
	;Load color
	cmp byte [si], 0xF0					; Is 0xF0?
	je .done						; If yes, done
	mov dl,byte [si]
	inc si
	;X Position
	mov ch,byte [si]
	inc si
	;Y Position
	mov cl,byte [si]
	
	push 0x81						;Draw pixel
	int 0x80
	inc si
	jmp .loop
	
.done:
	pop ax
	pop ds
	push 0x18		; Pause
	int 0x80

	push 0x82		; Exit graphics mode
	int 0x80
	jmp prompt_loop
	
.missing_parameter:

	mov si, .missing_parameter_msg
	push 0x02
	int 0x80
	jmp prompt_loop

.failure:

	mov si, .failure_msg
	push 0x02
	int 0x80
	jmp prompt_loop





.missing_parameter_msg	db 'Required parameter missing: file name.', 0x0A, 0x00
.failure_msg			db 'File not found.', 0x0A, 0x00
.failure_not_image_msg	db 'This file is not an image.', 0x0A, 0x00
