print_integer:

;   Prints a 32-bit integer number on screen.
;     number in EAX
;     DL = 1 if right align, 0 if not
;     CL = minimum number of digits to display (pads with ASCII 0s)

push eax
push ebx
push ecx
push edx
push di
push si
push ds

mov bx, 0x9000
mov ds, bx				; Point to kernel space

xor ch, ch
cmp dl, 1
je .right_align

mov dl, 0xFF
jmp .main

.right_align:
mov dl, ' '

.main:
push cx

.clear_buffer:
	mov cx, 10
	mov di, .buffer
	.clear_buffer_loop:
		mov byte [di], dl
		inc di
	loop .clear_buffer_loop

; Pad with 0s

pop cx
test cl, cl
jz .calculate
mov di, (.buffer+9)
.pad:
mov byte [di], '0'
dec di
loop .pad

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
