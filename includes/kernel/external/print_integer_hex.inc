print_integer_hex:

;   Prints a 32-bit integer number on screen in hexadecimal notation.
;     number in EAX
;     DL = 0 if not aligned or 1 if right aligned

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
