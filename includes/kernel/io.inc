;********************************************
;**             BASIC I/O APIs             **
;********************************************

; function_print_integer:
;   Prints a 32-bit integer number on screen.
;     number in EAX
;     DL = 0 if not aligned or 1 if right aligned

; function_print_integer_hex:
;   Prints a 32-bit integer number on screen in hexadecimal notation.
;     number in EAX
;     DL = 0 if not aligned or 1 if right aligned

; function_input_integer:
;   Gets a 32-bit integer from keyboard.
;     returns number in EAX

; function_input_string:
;   Gets a string from keyboard (0x00 terminated).
;     maximum string length in BX
;     buffer to save string to in ES:DI

; function_compare_strings:
;   Compares 2 strings (0x00 terminated).
;     strings in DS:SI and ES:DI
;     returns carry if the strings are equal

; function_string_length:
;   Returns the length of a string (0x00 terminated).
;     string in DS:SI
;     returns length in CX

; function_lower_to_uppercase:
;   Transform string to UPPERCASE (0x00 terminated).
;     string in ES:DI

; function_upper_to_lowercase:
;   Transform string to LOWERCASE (0x00 terminated).
;     string in ES:DI

; function_string_to_fat_name:
;	Trasform readable string (ex. 'photo.img') to FAT file name (ex. 'PHOTO   IMG').
;     Input string in DS:SI (0x00 terminated)
;     Output string in ES:DI (0x00 terminated)
;       THE BUFFER SIZE SHOULD ALWAYS BE 12 BYTES, SINCE THE FILE NAME IS 11 + 0x00

; function_fat_name_to_string:
;	Trasform FAT file name (ex. 'PHOTO   IMG') to a readable format (ex. 'PHOTO.IMG').
;     Input string in DS:SI (0x00 terminated)
;     Output string in ES:DI (0x00 terminated)
;       THE BUFFER SIZE SHOULD ALWAYS BE 13 BYTES, SINCE THE FILE NAME IS 12 + 0x00

; function_pause:
;	Pause the system until a key is pressed.

; function_string_to_integer:
;	Converts a numeric string into an integer
;     Input string in DS:SI (0x00 terminated)
;     Output integer in EAX

; function_string_copy:
;   Copies a string
;     Input string in DS:SI, copies into ES:DI (0x00 terminated)

; function_fat_time_to_integer:
;   Transforms raw FAT time into usable integers
;     AX = FAT time, BX = FAT date
;     Returns:
;     AL = Seconds
;     AH = Minutes
;     BL = Hours
;     BH = Days
;     CL = Months
;     DX = Years

function_print_integer:

	push eax
	push ebx
	push ecx
	push edx
	push di
	push si
	push ds

	mov bx, 0x9000
	mov ds, bx				; Point to kernel space

	cmp dl, 1
	je .right_align

	mov dl, 0xFF
	jmp .clear_buffer

	.right_align:
	mov dl, ' '

	.clear_buffer:
		mov cx, 10
		mov di, .buffer
		.clear_buffer_loop:
			mov byte [di], dl
			inc di
		loop .clear_buffer_loop
		jmp .calculate

	.calculate:
	mov di, (.buffer+9)
	mov byte [di], '0'
	.loop:
		xor edx, edx
		mov ebx, 10
		cmp eax, 0x00
		je .done
		div ebx
		add dl, '0'
		mov byte [di], dl
		dec di
		jmp .loop
	.done:
		mov si, .buffer
		push 0x02
		int 0x80

		pop ds
		pop si
		pop di
		pop edx
		pop ecx
		pop ebx
		pop eax
		iret
	.buffer times 10 db 0x00
				     db 0x00

function_print_integer_hex:

	push eax
	push ebx
	push ecx
	push edx
	push di
	push si
	push ds

	mov bx, 0x9000
	mov ds, bx				; Point to kernel space

	cmp dl, 1
	je .right_align

	mov dl, 0xFF
	jmp .clear_buffer

	.right_align:
	mov dl, ' '

	.clear_buffer:
		mov cx, 8
		mov di, .buffer
		.clear_buffer_loop:
			mov byte [di], dl
			inc di
		loop .clear_buffer_loop
		jmp .calculate

	.calculate:
	mov di, (.buffer+7)
	mov byte [di], '0'
	.loop:
		xor edx, edx
		mov ebx, 16
		cmp eax, 0x00
		je .done
		div ebx
		add dl, '0'
		cmp dl, '9'
		jg .adjust_for_ascii
	.adjusted:
		mov byte [di], dl
		dec di
		jmp .loop
	.adjust_for_ascii:
		add dl, 7
		jmp .adjusted
	.done:
		mov si, .buffer
		push 0x02
		int 0x80

		pop ds
		pop si
		pop di
		pop edx
		pop ecx
		pop ebx
		pop eax
		iret
	.buffer times 8 db 0x00
				    db 0x00

function_input_integer:

	push ebx
	push ecx
	push edx
	push si
	push di
	push ds
	push es

	mov ax, 0x9000
	mov ds, ax
	mov es, ax

	xor cx, cx
	xor dx, dx
	mov di, .buffer
	.loop:
		mov ah, 0x00
		int 0x16
		cmp al, 0x08
		je .backspace
		cmp al, 0x0D
		je .enter
		cmp al, '0'
		jl .loop
		cmp al, '9'
		jg .loop
		cmp cl, 10
		je .loop
		inc cl
		push 0x01
		int 0x80
		sub al, '0'
		stosb
		jmp .loop
	.backspace:
		cmp cl, 0
		je .loop
		mov al, 0x08
		push 0x01
		int 0x80
		dec di
		dec cl
		jmp .loop
	.enter:
		mov si, .buffer
		xor eax, eax
	.ascii_to_integer:
		cmp cl, 0x00
		je .done
		mov edx, 10
		mul edx
		xor edx, edx
		mov dl, byte [si]
		inc si
		add eax, edx
		dec cl
		jmp .ascii_to_integer
	.done:
		pop es
		pop ds
		pop di
		pop si
		pop edx
		pop ecx
		pop ebx
		iret
	.buffer times 10 db 0x00

function_input_string:

	push ax
	push bx
	push cx
	push di

	xor cx, cx
	.loop:
		mov ah, 0x00
		int 0x16
		cmp al, 0x08
		je .backspace
		cmp al, 0x0D
		je .enter
		cmp cx, bx
		je .loop
		inc cx
		push 0x01
		int 0x80
		stosb
		jmp .loop
	.backspace:
		cmp cx, 0
		je .loop
		mov al, 0x08
		push 0x01
		int 0x80
		dec di
		dec cx
		jmp .loop
	.enter:
		mov al, 0x00
		stosb
		pop di
		pop cx
		pop bx
		pop ax
		iret

function_compare_strings:

	push ax
	push si
	push di
	push bp
	.loop:
		lodsb
		mov ah, byte [es:di]
		inc di
		cmp al, ah
		jne .not_equal
		test al, al
		jz .equal
		jmp .loop

	.not_equal:
		mov bp, sp
		add sp, 12		; Get to the flags
		pop ax			; Pop and clear carry flag
		and ax, 1111111111111110b
		push ax			; Put flags back in stack
		mov sp, bp		; Restore stack
		jmp .done

	.equal:
		mov bp, sp
		add sp, 12		; Get to the flags
		pop ax			; Pop and set carry flag
		or ax, 0000000000000001b
		push ax			; Put flags back in stack
		mov sp, bp		; Restore stack
		jmp .done

	.done:
		pop bp
		pop di
		pop si
		pop ax
		iret

function_string_length:

	push ax
	push si

	xor cx, cx
	.loop:
		lodsb			; Byte from SI
		test al, al		; Is 0?
		jz .done
		inc cx
		jmp .loop

	.done:
	pop si
	pop ax
	iret

function_lower_to_uppercase:

    push ax
	push di

    .loop:
		mov al, byte [es:di]	; Byte from DI
		cmp al, 0x60
		jg .convert
		cmp al, 0x00
		je .done
	.no_convert:
		inc di
		jmp .loop

	.convert:
		cmp al, 0x7A
		jg .no_convert
		sub al, 0x20
		stosb
		jmp .loop

	.done:
		pop di
		pop ax
		iret

function_upper_to_lowercase:

    push ax
	push di

    .loop:
		mov al, byte [es:di]	; Byte from DI
		cmp al, 0x40
		jg .convert
		cmp al, 0x00
		je .done
	.no_convert:
		inc di
		jmp .loop

	.convert:
		cmp al, 0x5A
		jg .no_convert
		add al, 0x20
		stosb
		jmp .loop

	.done:
		pop di
		pop ax
		iret

function_string_to_fat_name:

	push ax
	push cx
	push si
	push di

	mov cl, 11

	.clear_buffer:
		cmp cl, 0
		je .main
		mov al, ' '
		stosb
		dec cl
		jmp .clear_buffer

	.main:
		pop di
		push di
		mov cl, 11

	.loop:
		lodsb			; Byte from SI
		cmp al, '.'		; Is '.'?
		je .pad_and_extension
		cmp al, 0x00	; Is terminator?
		je .pad_and_convert
		cmp cl, 3		; Too may characters?
		je .convert		; Quit
		stosb			; Save in DI
		dec cl
		jmp .loop

	.pad_and_extension:
		sub cl, 3		; Sub extension size from padding
	.loop1:
		cmp cl, 0
		je .put_extension
		mov al, ' '		; Pad with spaces
		dec cl
		stosb
		jmp .loop1
	.put_extension:
		mov cl, 3
	.loop2:
		cmp cl, 0
		je .convert
		lodsb			; Byte from SI
		cmp al, 0x00	; Is 0x00?
		je .pad_and_convert
		stosb			; Store in DI
		dec cl
		jmp .loop2

	.pad_and_convert:
		cmp cl, 0
		je .convert
		mov al, ' '		; Pad with spaces
		stosb
		dec cl
		jmp .pad_and_convert

	.convert:
		pop di			; Reset DI
		push di
		add di, 11
		mov byte [es:di], 0x00	; Add 0x00 terminator
		sub di, 11
		push 0x15
		int 0x80		; Lower to uppercase

	.done:
		pop di
		pop si
		pop cx
		pop ax
		iret

function_fat_name_to_string:

	push ax
	push cx
	push si
	push di

	mov cx, 8		; The name is 8 characters max
	.name_loop:
		lodsb
		cmp al, ' '		; Check for space
		je .skip_padding_loop
		stosb
		loop .name_loop

	.skip_padding_loop:
		mov al, byte [ds:si]
		test al, al
		jz .done		; No extension
		cmp al, ' '
		jne .extension
		inc si
		jmp .skip_padding_loop

	.extension:
		mov al, '.'
		stosb

	.extension_loop:
		lodsb
		cmp al, ' '
		je .done
		test al, al
		jz .done
		stosb
		jmp .extension_loop

	.done:
		xor al, al		; Add terminator
		stosb

		pop di
		pop si
		pop cx
		pop ax
		iret
		
function_pause:

	push ax
	
	xor ah, ah
	int 0x16
	
	pop ax
	iret

function_string_to_integer:

	push ebx
	push ecx
	push edx
	push si

	xor ecx, ecx			; Prepare ECX

	.loop:
		xor eax, eax		; Prepare EAX
		lodsb				; Get byte from string
		sub al, '0'			; Get integer value
		mov dl, byte [si]	; Check whether this is the last byte in the string
		test dl, dl			; Is the string terminated?
		jz .done			; If yes, we're done
		add ecx, eax		; Add new value to stored result
		mov eax, 10
		mul ecx				; Multiply result by 10
		mov ecx, eax		; Store result back in ECX
		jmp .loop			; Loop

	.done:
		add ecx, eax		; Add last unit
		mov eax, ecx		; Move result into EAX
		pop si
		pop edx
		pop ecx
		pop ebx
		iret

function_string_copy:

	push ax

	.loop:
		lodsb
		stosb
		test al, al
		jz .done
		jmp .loop

	.done:
		pop ax
		iret

function_fat_time_to_integer:

	push ds

	mov dx, 0x9000
	mov ds, dx

	mov word [.RawTime], ax
	mov word [.RawDate], bx

	; Convert seconds

	mov ax, word [.RawTime]

	and ax, 0000000000011111b		; Extract seconds/2
	mov bx, 2						; Seconds*2
	mul bx

	mov byte [.Seconds], al			; Save

	; Convert minutes

	mov ax, word [.RawTime]

	and ax, 0000011111100000b		; Extract minutes
	shr ax, 5						; Adjust it

	mov byte [.Minutes], al			; Save

	; Convert hours

	mov ax, word [.RawTime]

	and ax, 1111100000000000b		; Extract hours
	shr ax, 11						; Adjust it

	mov byte [.Hours], al			; Save

	; Convert day

	mov ax, word [.RawDate]

	and ax, 0000000000011111b		; Extract day

	mov byte [.Day], al				; Save

	; Convert month

	mov ax, word [.RawDate]

	and ax, 0000000111100000b		; Extract month
	shr ax, 5						; Adjust it

	mov byte [.Month], al			; Save

	; Convert year

	mov ax, word [.RawDate]

	and ax, 1111111000000000b		; Extract year-1980
	shr ax, 9						; Adjust it
	add ax, 1980					; Add 1980

	mov word [.Year], ax			; Save

	; *** Copy results in the proper registers ***

	mov al, byte [.Seconds]
	mov ah, byte [.Minutes]
	mov bl, byte [.Hours]
	mov bh, byte [.Day]
	mov cl, byte [.Month]
	mov dx, word [.Year]

	pop ds
	iret

	.RawDate	dw 0x0000
	.RawTime	dw 0x0000
	.Seconds	db 0x00
	.Minutes	db 0x00
	.Hours		db 0x00
	.Day		db 0x00
	.Month		db 0x00
	.Year		dw 0x0000
